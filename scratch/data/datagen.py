import sys
from random import randint
from secrets import token_hex

if len(sys.argv) != 3:
	print("python3 testgen.py <number of write batches> <number of ops in a batch>")
	sys.exit(1)

#sys.argv[1] is the number of batches, sys.argv[2] is the number of operations in each

# schema = open("schema.sql", "w")

# schema.write('''
# drop table if exists testmultiple;
# create table testmultiple(
# 	keycol varchar(64) primary key,
# 	valcol int
# );
# ''')

# schema.close()

batches = int(sys.argv[1])
batchsize = int(sys.argv[2])

for batch in range(batches):
	#this generates writes, and the reads expected to return non-null stuff
	#higher batch numbers are more recently touched stuff
	writefile = open("writebatch"+str(batch)+".sql", "w")
	read1file = open("read1batch"+str(batch)+".sql", "w")
	read2file = open("read2batch"+str(batch)+".sql", "w")
	for op in range(batchsize):
		k = token_hex(32)
		v = token_hex(32)
		writefile.write("w "+str(k)+", "+str(v)+";\n")
		if op%4 == 0:
			read2file.write("r "+str(k)+";\n")
		if op%10 == 0:
			k1 = token_hex(32)
			read1file.write("r ="+str(k1)+";\n")
	writefile.close()
	read1file.close()
	read2file.close()
