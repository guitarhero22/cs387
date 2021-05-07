#include "master.hpp"


bool Master::dbread(const K &k, V &v){

	//what is recent now
	this->memlock.lock();
	int r = this->recent;

	//check bloom filter
	if (!this->bloom->get(k)){
		this->memlock.unlock();
		return false;
	}
	
	//check in memory tree, and reserve too, since you hold lock
	Node<K, V> *n = this->bintree->search(k);
	Node<K, V> *nr = this->reserve->search(k);
	this->memlock.unlock();
	
	if (n != NULL){
		
		memcpy(&v, &n->value, sizeof(V));
		return true;
	}
	if (nr != NULL)
	{

		memcpy(&v, &nr->value, sizeof(V));
		return true;
	}

	//now traverse filesystem
	for (int i=r; i!=(r+2)%NUMFILES; i=(i-1)%NUMFILES){
		int fd = FileIO::openFile(filesys[i]);
		if (findEntry<K, V>(fd, k, v))
		{
			return true;
		}
	}

	//last two danger files. if recent has been updated,
	//last is redundant, becuase we already checked reserve too
	this->fslock.lock();
	for (int i = (r + 2) % NUMFILES; i != r; i = (i - 1) % NUMFILES)
	{
		int fd = FileIO::openFile(filesys[i]);
		if (findEntry<K, V>(fd, k, v))
		{
			return true;
		}
	}
	this->fslock.unlock();
	return false;
}

void Master::dbwrite(K k, V v){
	
	//acquire the memory lock
	this->memlock.lock();

	//write to memory and backup
	this->bintree->insert(k, v);

	//write to Bloom
	this->bloom->set(k);

	//release the memory lock
	this->memlock.unlock();
}

void Master::serve(string batchfile){
	string a;

	ifstream f1(batchfile);
	while (getline(f1, a))
	{
		if (a[0] == 'r')
		{
			string x, y, z;
			x = a.substr(0, a.find(' '));
			y = a.substr(a.find(' ') + 1, 64);
			K K1;
			V V1;
			K1 = *(K *)y.c_str();
			//	for(int i=0;i<1;i++) cout<<K1.bytes[i];
			this->dbread(K1, V1);
			//	cout<<y<<endl;
		}
		
		else if (a[0] == 'w')
		{
			string x, y, z;
			x = a.substr(0, a.find(' '));
			y = a.substr(a.find('w') + 2, 64);
			z = a.substr(a.find(',') + 2, 64);
			K K1;
			V V1;
			K1 = *(K *)y.c_str();
			V1 = *(V *)z.c_str();
			//for(int i=0;i<1;i++) cout<<K1.bytes[i];cout<<endl;
			this->dbwrite(K1, V1);
			//cout<<y<<" "<<z<<endl;
		}

		this->memlock.lock();
		if (this->bintree->size >= 2048 && this->reserve->size == 0)
		{

			this->reserve = this->bintree;
			this->current = (this->current + 1) % 2;
			this->bintree = new BinaryTree<K, V>(backups[current]);
			this->memlock.unlock();
			thread(&Master::adjust, this).detach();
		}
		else
		{
			this->memlock.unlock();
		}
	}

}

void Master::adjust(){
	
	//acquire the file lock
	this->fslock.lock();

	//empty tempfile
	filesystem::resize_file(tempfile, 0);
	
	//copy the second oldest file into tempfile
	ifstream src(filesys[(this->recent+2)%NUMFILES], std::ios::binary);
	ofstream dst(this->tempfile, std::ios::binary);
	dst << src.rdbuf();
	src.close();
	dst.close();

	//overwrite the second oldest file with the merge
	filesystem::resize_file(filesys[(this->recent + 2) % NUMFILES], 0);
	FILE *oldest2 = fopen(tempfile.c_str(), "rb");
	FILE *oldest1 = fopen(filesys[(this->recent + 1) % NUMFILES].c_str(), "rb");
	FILE *newfd = fopen(filesys[(this->recent + 2) % NUMFILES].c_str(), "wb");
	merge_files<K, V>(oldest2, oldest1, newfd);
	fclose(oldest2);
	fclose(oldest1);
	fclose(newfd);

	//empty tempfile
	filesystem::resize_file(tempfile, 0);

	//dump inorder traversal
	FILE *f = fopen(filesys[(this->recent + 1) % NUMFILES].c_str(), "wb");
	this->reserve->dump(f);
	fclose(f);

	//update recent
	this->memlock.lock();
	this->recent = (this->recent + 1)%NUMFILES;

	//clear the contents of the old backup
	filesystem::resize_file(backups[(this->current+1)%2], 0);

	//can finally reclaim memory
	delete this->reserve;
	this->reserve = NULL;
	this->memlock.unlock();

	//release the file lock
	this->fslock.unlock();

}

void Master::run(vector <string> batchfiles){
	int numthreads = batchfiles.size();
	vector <thread> threads;
	for (int i=0; i<numthreads; i++){
		threads.push_back(thread(&Master::serve, this, batchfiles[i])); 
	}

	for (auto &t : threads)
		t.join();
}

Master::Master(){

	//assumes that file<i> begins with a timestamp

	//what files are we accessing?
	string fs = "file";
	for (int i=0; i<NUMFILES; i++){
		filesys[i] = fs+to_string(i);
	}
	
	string back = "backup";
	for (int i=0; i<2; i++){
		backups[i] = back+to_string(i);
	}

	bloomdump = "bloomdump";

	tempfile = "tempfile";

	//what is recent?

	FILE *zeroth = fopen(filesys[0].c_str(), "rb");
	fseek(zeroth, 0, SEEK_SET);
	char time_str0[128];
	fread(time_str0, 1, 128, zeroth);
	fclose(zeroth);
	tm ts_max;
	int candidate = 0;
	strptime(time_str0, TIME_FMT, &ts_max);

	for (int i=1; i<NUMFILES; i++){
		FILE *ith = fopen(filesys[i].c_str(), "rb");
		fseek(ith, 0, SEEK_SET);
		char time_stri[128];
		fread(time_stri, 1, 128, ith);
		fclose(ith);
		tm ts_i;
		strptime(time_stri, TIME_FMT, &ts_i);

		if (difftime(mktime(&ts_max), mktime(&ts_i)) < 0)
		{
			candidate = i;
			memcpy(&ts_max, &ts_i, sizeof(tm));
		}
	}

	recent = candidate;

	//write backups, if any, to file

	for (int i=0; i<2; i++){
		this->reserve = new BinaryTree<K, V>(tempfile);
		current = (i+1)%2; 
		//ensures the erase current(i+1)%2 step in adjust() gets rid of the used backup
		this->reserve->fromFile(backups[i]);
		
		adjust();

	}

	current = 0;
	this->bintree = new BinaryTree<K, V>(backups[0]);

	//boot Bloom
	this->bloom = new BloomFile<K>(bloomdump, false, true);
}