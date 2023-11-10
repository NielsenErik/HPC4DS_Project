COMMANDS FOR TERMINAL:
scripts/compile.sh

How to run serial examples:
Change the first "for" in scripts/serial/run_all_serials.sh cause the default version would not success in running so many programs (due to HPC user limitations).
For example run this script with N=1 until N <=2, then again with N=4 until N<=8 and so on.
scripts/serial/run_all_serials.sh

You're gonna obtain multiple results. To copy those results inside the Google sheet, start using:
scripts/serial/merge_serial_results.sh
This will produce serial_results.txt.
Copy its content and place it in this formula:
=SPLIT("<content_you_copied>", ",")
then take it and put it in cell C4



How to run parallel examples:
Same as serial, but:
- you'll have more of them so you're gonna edit scripts/run_all_parallels.sh multiple times in order to change the number of processes involved as well. 
	Take a look at that script and if it's not clear, contact me.
- merge_parallel_results.sh is an old version so do not use it.
	
	
For every other question, do not hesitate to contact me.