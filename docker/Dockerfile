FROM rockylinux/rockylinux:latest
ENV container docker
# gcc
RUN dnf install gcc -y
# make
RUN dnf install epel-release -y
RUN dnf install make -y
RUN dnf install findutils -y
#
RUN dnf install 'dnf-command(config-manager)'
RUN dnf config-manager --set-enabled powertools# unittest
# RUN dnf install libtool -y
RUN dnf install unittest* -y
# git
RUN dnf install git -y



RUN export LD_LIBRARY_PATH=/usr/local/custom/lib/
