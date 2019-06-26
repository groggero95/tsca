#
# Copyright (C) EURECOM, Telecom Paris
#
# Prof. Renaud PACALET          <renaud.pacalet@telecom-paristech.fr>
# Alberto ANSELMO               <Alberto.Anselmo@eurecom.fr>
# Simone Alessandro CHIABERTO	<Simone-Alessandro.Chiaberto@eurecom.fr>
# Fausto CHIATANTE              <Fausto.Chiatante@eurecom.fr>
# Giulio ROGGERO                <Giulio.Roggero@eurecom.fr>
#
# This file must be used under the terms of the CeCILL. This source
# file is licensed as described in the file COPYING, which you should
# have received as part of this distribution. The terms are also
# available at:
# http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.txt 
#

TOP ?= model.pdf

FIGS       = $(wildcard figs/*.fig)
FIGS_PDF   = $(patsubst %.fig,%.pdftex,$(FIGS))
FIGS_PDF_T = $(patsubst %.fig,%.pdftex_t,$(FIGS))

SVGS       = $(wildcard svgs/*.svg)
SVGS_PDF   = $(patsubst %.svg,%.pdf,$(SVGS))

ODGS       = $(wildcard odgs/*.odg)
ODGS_PDF   = $(patsubst %.odg,%.pdf,$(ODGS))

NEEDED = $(FIGS_PDF) $(FIGS_PDF_T) $(SVGS_PDF) $(ODGS_PDF)

TOP_NAME = $(patsubst %.pdf,%,$(TOP))
EXT = pdf log aux out bbl blg toc snm nav fdb_latexmk fls

PREVIEW_OPTS  = \RequirePackage[active,delayed,tightpage,graphics,pdftex] {preview}
PREVIEW_OPTS += \PreviewMacro[{*[][]{}}]{\incode}

export TEXINPUTS := ./texinputs/:$(TEXINPUTS)

#.SECONDARY: $(FIGS_PDF)

.PHONY: all clean toc bibetex

all: $(TOP)

$(TOP) : $(NEEDED)

%.pdf:%.tex
	@echo "Latex search path $(TEXINPUTS)"
	@latexmk -pdf $<

toc: $(TOP)
	@pdflatex $(patsubst %.pdf,%.tex,$<)
bibtex: $(TOP)
	@bibtex   $(patsubst %.pdf,%,$<)
	@pdflatex $(patsubst %.pdf,%.tex,$<)
	@pdflatex $(patsubst %.pdf,%.tex,$<)

preview: $(NEEDED)
	pdflatex  '$(PREVIEW_OPTS) \input{$(TOP_NAME).tex}'

clean:
	@echo Cleaning $(TOP) and $(NEEDED)
	@$(foreach ext,$(EXT),[ -e $(TOP_NAME).$(ext) ] && rm -f $(TOP_NAME).$(ext) || true;)
	@rm -f $(NEEDED)
	@$(foreach ext,$(EXT),[ -e preview.$(ext) ] && rm -f preview.$(ext) || true;)


%.pdftex:%.fig
	fig2dev -L pdftex $< $@
%.pdftex_t:%.pdftex
	fig2dev -L pdftex_t -p $< $(patsubst %.pdftex_t,%.fig,$@) $@

%.swf:%.pdf
	pdf2swf $< && chmod -x $@

%.pdf:%.svg
	inkscape -f $< --export-pdf=$@

%.pdf:%.odg
	libreoffice --headless --convert-to pdf $< --outdir odgs
	pdfcrop --margins 1 $@ $@
