# base image
FROM python:3.11.4-slim-bullseye

# install apt packages
RUN apt -y update
RUN apt -y upgrade
RUN apt -y install build-essential git cmake ninja-build graphviz graphviz-dev clang
