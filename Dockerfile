FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       ca-certificates \
       git \
       make \
       build-essential \
       pkg-config \
       gcc \
       g++ \
       libssl-dev \
       libhpdf-dev \
       ghostscript \
       inkscape \
       latexmk \
       texlive-full \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work

# Usage (from repo root):
#   docker build -t mega65-docs .
#   docker run --rm -v "$PWD":/work -w /work mega65-docs make mega65-book.pdf


