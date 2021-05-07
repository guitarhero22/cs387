import sys
from random import randint
from secrets import token_hex

if len(sys.argv) != 3:
	print("python3 testgen.py <number of write batches> <number of ops in a batch>")
	sys.exit(1)

#sys.argv[1] is the number of batches, sys.argv[2] is the number of operations in each

batches = int(sys.argv[1])
batchsize = int(sys.argv[2])
schema = open("schema.sql", "w")
lsmschema = open("lsmschema.sql", "w")

lsmschema.write('''
drop table if exists testmultiple;
drop extension lsm3 cascade;
create extension lsm3;
create table testmultiple(
	keycol varchar(64),
	valcol varchar(64)
);''')

schema.write('''
drop table if exists testmultiple;
create table testmultiple(
	keycol varchar(64) primary key,
	valcol varchar(64)
);\n
''')
schema.close()
lsmschema.close()

for batch in range(batches):
	#this generates writes, and the reads expected to return non-null stuff
	#higher batch numbers are more recently touched stuff
	writefileb = open("bwritebatch"+str(batch)+".sql", "w")
	read1fileb = open("bread1batch"+str(batch)+".sql", "w")
	read2fileb = open("bread2batch"+str(batch)+".sql", "w")
	writefileb.write("insert into testmultiple values")

	writefilecsv = open("writebatch"+str(batch)+".csv", "w")
	read1filecsv = open("read1batch"+str(batch)+".csv", "w")
	read2filecsv = open("read2batch"+str(batch)+".csv", "w")

	writefiles = open("writebatch"+str(batch)+".scratch", "w")
	read1files = open("read1batch"+str(batch)+".scratch", "w")
	read2files = open("read2batch"+str(batch)+".scratch", "w")

	writefile = open("writebatch"+str(batch)+".sql", "w")
	read1file = open("read1batch"+str(batch)+".sql", "w")
	read2file = open("read2batch"+str(batch)+".sql", "w")

	for op in range(batchsize):
		k = token_hex(32)
		v = token_hex(32)
		# writefile.write("insert into testmultiple values('%s', '%s') on conflict(keycol) do update set valcol = '%s';\n" %(str(k), str(v), str(v)))
		writefile.write("insert into testmultiple values('%s', '%s');\n" %(str(k), str(v)))
		writefileb.write("\n,('%s', '%s')" %(str(k), str(v)))
		writefilecsv.write("%s, %s\n" %(str(k), str(v)))
		writefiles.write("w %s, %s;\n" %(str(k), str(v)))
		if op%4 == 0:
			read2file.write("select * from testmultiple where keycol = '%s';\n"%str(k))
			read2fileb.write("select * from testmultiple where keycol = '%s';\n"%str(k))
			read2filecsv.write("%s\n"%str(k))
			read2files.write("r %s\n"%str(k))
		if op%10 == 0:
			k1 = token_hex(32)
			read1file.write("select * from testmultiple where keycol = '%s';\n"%str(k1))
			read1fileb.write("select * from testmultiple where keycol = '%s';\n"%str(k1))
			read1filecsv.write("%s\n"%str(k1))
			read1files.write("r %s;\n"%str(k1))

	writefileb.write(";")

