ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
FACTORY=docker
OPTIONS=run -it --rm -v "$(ROOT_DIR)":"$(ROOT_DIR)":Z -w "$(ROOT_DIR)" ghcr.io/csc-training/slidefactory:3.3.1

SRC=$(wildcard *.md)
HTML=$(patsubst %.md,%.html,$(SRC))
PDF=$(patsubst %.md,%.pdf,$(SRC))

.PHONY: html pdf clean

html: $(HTML)

pdf: $(PDF)

clean:
	-rm -f $(HTML) $(PDF)

%.html: %.md
	$(FACTORY) $(OPTIONS) slides --format html $<

%.pdf: %.md
	$(FACTORY) $(OPTIONS) slides --format pdf $<
