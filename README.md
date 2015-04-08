# stdf2xml

First git project

The idea is to a create a C++ tool to convert STDF v4 files to a more "human-friendly" XML. 

**pugixml** was used to create the nodes.

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
