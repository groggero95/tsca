TOP ?= document.pdf

#FIGS       = $(wildcard figs/*.fig)
#FIGS_PDF   = $(patsubst %.fig,%.pdftex,$(FIGS))
#FIGS_PDF_T = $(patsubst %.fig,%.pdftex_t,$(FIGS))

PNG       	= $(wildcard graphics/*.png)
PNG_EPS   	= $(patsubst %.png, %.eps, $(PNG))

EPS			= $(wildcard graphics/*.eps)
EPS_PDF		= $(patsubst %.eps, %.pdf, $(EPS))

#ODGS       = $(wildcard odgs/*.odg)
#ODGS_PDF   = $(patsubst %.odg,%.pdf,$(ODGS))

#NEEDED = $(FIGS_PDF) $(FIGS_PDF_T) $(SVGS_PDF) $(ODGS_PDF)
NEEDED 		= $(PNG_EPS) $(EPS_PDF)

TOP_NAME 	= $(patsubst %.pdf,%,$(TOP))
EXT 		= pdf log aux out bbl blg toc snm nav fdb_latexmk fls dvi vrb
EXT_TMP		= log aux out bbl blg toc snm nav fdb_latexmk fls vrb

PREVIEW_OPTS  = \RequirePackage[active,delayed,tightpage,graphics,pdftex] {preview}
PREVIEW_OPTS += \PreviewMacro[{*[][]{}}]{\incode}

export TEXINPUTS := ./texinputs/:$(TEXINPUTS)

#.SECONDARY: $(FIGS_PDF)

.PHONY: all clean toc bibtex tmp_clean view_doc

all: $(TOP) view_doc

pres: $(TOP)

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

tmp_clean:
	@echo Cleaning temporary files
	@$(foreach ext,$(EXT_TMP),[ -e $(TOP_NAME).$(ext) ] && rm -f $(TOP_NAME).$(ext) || true;)
	@$(foreach ext,$(EXT_TMP),[ -e preview.$(ext) ] && rm -f preview.$(ext) || true;)

view_doc:
	xdg-open $(TOP) && evince $(TOP) &

%.eps:%.png
	convert $< $@

%.pdf:%.eps
	inkscape -z -f $< --export-pdf=$@ -z
