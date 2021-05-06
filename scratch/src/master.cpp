#include "master.hpp"

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
	//TODO: figure out a way to read and parse batchfile neatly
	while(1){
		//condition to be replaced with (getline) or sth

		this->memlock.lock();
		
		//call whatever read/write

		
		if (this->bintree->size >= 2048 && this->reserve->size == 0){
		
			this->reserve = this->bintree;
			this->current = (this->current + 1)%2;
			this->bintree = new BinaryTree<K, V>(backups[current]);
			this->memlock.unlock();
			thread (adjust).detach();
		}
		else{
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
	this->memlock.unlock();

	//release the file lock
	this->fslock.unlock();

	
}