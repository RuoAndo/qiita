A = LOAD 'input/random_data.txt' USING PigStorage(',') as (pdfdata:CHARARRAY);
Dump A;
