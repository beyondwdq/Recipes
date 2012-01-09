TEX_NAME = replies_to_review_comments

all:
	latex ${TEX_NAME}.tex
	dvipdf ${TEX_NAME}.dvi

.PHONY : clean
clean : 
	-del.sh ${TEX_NAME}.log ${TEX_NAME}.out ${TEX_NAME}.aux ${TEX_NAME}.bbl ${TEX_NAME}.blg ${TEX_NAME}.dvi 
