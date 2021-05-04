FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get upgrade && apt-get install -y gnupg2 lsb-release git gcc make wget
RUN wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc |\
    apt-key add - &&\
    echo "deb http://apt.postgresql.org/pub/repos/apt/ `lsb_release -cs`-pgdg main" > /etc/apt/sources.list.d/pgdg.list &&\
    apt-get update && echo "1\n1" | apt-get install -y postgresql-13
