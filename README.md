# stdf2xml

First git project

The idea is to a create a C++ tool to convert STDF v4 files to a more "human-friendly" XML. 

**pugixml** was used to create the nodes.

How to use?
1. stdf2xml <path input stdf file> <path output xml file>
2. Wait until it's done... It can take several minutes depending on the size of the input file.
3. Warning: the XML can be REALLY big. For ~300MB STDF files it is possible to get files with over 2GB size.


STDF records implemented so far:
- FAR
- ATR
- MIR
- MRR
- PCR
- HBR
- SBR
- PMR
- PGR
- PLR
- RDR
- SDR
- WIR
- WRR
- WCR
- PIR
- PRR
- TSR
- PTR
- MPR
- FTR
- BPS
- EPS
- DTR

Records not implemented:
- GDR

Missing features:
- Conversion of big-endian STDF files
- Conversion of files whose version is not 4
