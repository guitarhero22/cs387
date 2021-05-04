FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get upgrade && apt-get install -y gnupg2 lsb-release git gcc make wget
RUN wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc |\
    apt-key add - &&\
    echo "deb http://apt.postgresql.org/pub/repos/apt/ `lsb_release -cs`-pgdg main" > /etc/apt/sources.list.d/pgdg.list &&\
    apt-get update && echo "1\n1" | apt-get install -y postgresql-13
RUN apt-get install -y postgresql-server-dev-13
COPY lsm3 /src/lsm3
WORKDIR /src/lsm3
RUN chmod 777 /src/lsm3
RUN make USE_PGXS=1 &&\
    make install USE_PGXS=1
RUN cat lsm3.conf >> /etc/postgresql/13/main/postgresql.conf &&\
    pg_ctlcluster start 13 main &&\
    service postgresql reload &&\
    su - postgres -c "cd /src/lsm3; make installcheck USE_PGXS=1" &&\
    service postgresql stop
WORKDIR ~