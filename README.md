## RVSWD Analyzer plugin for the Saleae Logic, for the WCH riscV chipsets

## Building

The Saleae SDK requires Python to script the build of an analyzer; if your computer doesn't have this installed, you are out of luck.

After downloading this project's code, issue the following command to download the SDK files from Saleae's github page:

```
git submodule update --init AnalyzerSDK
```

Read the file ./AnalyzerSDK/lib/readme.txt and follow its instructions.  It will direct you to rename libAnalyzer64.so to libAnalyzer.so if you are running 64-bit Linux.

To compile, run the following from the base directory of this project:

```
python build_analyzer.py
```

Consult the Saleae support web site and their [SampleAnalyzer](https://github.com/saleae/SampleAnalyzer) for any issues with the compilation procedure.

The end result of compilation is a library in the "SW-DP/release/" subdirectory.  This file must be copied to the "Analyzers" subdirectory of the Saleae Logic software.

## License

The contents of this repository are released under [LGPL 2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html).

