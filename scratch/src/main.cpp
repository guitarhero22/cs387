#include "master.hpp"

int main(int argc, char* argv[]){
	Master master = Master();
	vector <string> batchfiles;
	if (argc > 1)
	{
		batchfiles.assign(argv + 1, argv + argc);
	}
	master.run(batchfiles);
	sleep(1);
}