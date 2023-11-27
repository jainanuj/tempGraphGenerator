Creates a temporal graph from the static graph
The graphs created are in both models: temporal intervals and contact sequences.
Command to use:

./temporalGraphGenerator filename.txt \<avg_num_intvls\> \<avg_intvl_duration\> \<avg_trvlTime\> \<cs\>
Output will be in the files:

filename_op_final-numIntvls-intvlDuration-trvlTime.txt ---- for the temporal interval model.
filename_op_final-numIntvls-intvlDuration-trvlTime_wu.txt    ---- for the contact sequence model.
---------------------------------------

When the last "cs" param is used,
Output will be in the files:

filename_op_cs_final.txt ---- for the interval cont seq model.
filename_op_wu.txt    ---- for the contact sequence model.

