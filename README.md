# Timing Side Channel Attack

This project has been developed as a semester project during our studies at EURECOM, Sophia Antipholis (Fall 2018 - Spring 2019), with the supervision of Prof. Renaud Pacalet.

## Goal
The purpose of this project is to understand how it is possible, knowing the algorithm, to retrieve either the inputs or the outputs knowing the other counterparts. At the very beginning, we developed our own algorithm, running on a bare metal Zybo board which has an ARM-architecture CPU.

## Analysis
By statistically analyzing the time required by the HW to perform the needed operations, we are likely to retrieve informations regarding the data. As a matter of fact, the timing optimization depends on the data, which means that if we can get information regarding the execution time and/or the power consumption, it should be possible to know the operands with a significant certainty.


## Contacts
For any information, you can simply drop an email:
* [Alberto Anselmo](mailto:anselmo@eurecom.fr)
* [Simone Alessandro Chiaberto](mailto:chiabert@eurecom.fr)
* [Giulio Roggero](mailto:roggero@eurecom.fr)
