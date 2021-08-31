# Classroom COVID Simluation

This project used high-resolution observations of interaction to simulate infectious processes. 

The raw input data can be found at https://osf.io/jdbmt/?view_only=fe7c3af604ed40d389db0636aa599354. The following are steps to process the raw input data to the
output of the simulation.

1. Compile log2tag.cpp and tag2table.cpp, i.e., named as log2tag and tag2table, respectively;

2. Use log2tag to process the individual raw input file, i.e., ./log2tag raw_input.csv; 

3. Change content of the file named list.dat to as same format as indix_example.dat, and the list.dat was one output from step2; 
(NOTE: the labels in indix_example.dat will be subjects in the simulations, you can also choose some of them to be the participants)

4. Use tag2table to process the files output of step2, i.e., ./tag2table indix_example.dat; Then it outputs all_xy.csv file.

5. Lastly, run python script transmission_simulation.py by giving the '--half class' argument, i.e., it can be full/half determining the sizes of the classrooms.

6. Check output files, there will be 4 subfolders titled as 'full_class', 'full_class_vaccinated', 'half_class', and 'half_class_vaccinated'. Each of those
indicates the simulation scenario. Within each scenario subfolder, there are several subfolder labeled by 'zero-patient-indx'. For example, folder 'zero_patient_1',
it implies this is a assumption that zero patient was kid 13. You can find repeating simulations for each zero patient named as 'simulation-num.dat', like 'simulation1.csv'.
In each single output file, each line represents one hour infection status, '0' means staying susceptable, '1' indicates being infected, and '2' implies getting 
recovered, and the lines ordered by time. The simulations will stop when there is no futher infection(no '1's).
