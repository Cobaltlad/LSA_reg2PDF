# LSA_reg2pdf Project

## Introduction
LSA_reg2pdf is a tool designed to automate the extraction and retrieval of LSA (Local Security Authority) secrets from PDF documents. This project consists of two components: a C program (`get_pdf.c`) for extracting PDF files from a remote host, and a Python script (`get_bootkey.py`) for extracting the boot key from these PDF files.

The project is inspired by the methodology described in the article [Dumping LSA Secrets: A Story About Task Decorrelation](https://sensepost.com/blog/2024/dumping-lsa-secrets-a-story-about-task-decorrelation/) by [Aurélien CHALOT](https://github.com/Dfte) . It aims to facilitate remote extraction and retrieval of keys from PDF files, enhancing security analysis capabilities.

## Project Overview
- **C Program (`get_pdf.c`)**: Extracts PDF files containing LSA secret information from a remote host. This component must be run with Admin privileges on the target system to collect the necessary PDF files.
- **Python Script (`get_bootkey.py.`)**: Processes the extracted PDF files to extract the boot key related to LSA secrets. It uses Python scripting and libraries such as PyMuPDF for PDF handling and OCR (Optical Character Recognition) capabilities.

## Features
- **PDF Extraction (C Program)**: Automates the extraction of PDF files from a remote host.
- **PDF Text Extraction (Python Script)**: Extracts text from PDF files using PyMuPDF.
- **Key Extraction**: Identifies and extracts key information related to LSA secrets based on predefined patterns in the PDF documents.
- **Remote Accessibility**: Designed for remote usage scenarios, allowing for flexible deployment and operation.

## Usage
1. **Setup**:
   - Compile and run the `get_pdf.c` on the target system to extract PDF files containing LSA secret information.
   - Ensure Python environment with necessary dependencies (`PyMuPDF`, OCR libraries) for running
   ```bash
   pip install pymupdf
   sudo apt install mingw-w64
   x86_64-w64-mingw32-gcc get_pdf.c -o get_pdf.exe   -luser32 -lgdi32 -lwinspool -lcomdlg32 -luuid -loleaut32 -lole32

2. **Configuration**:
   - Modify `pdf_paths` variable in `get_bootkey.py` to specify paths to the extracted PDF files.

3. **Execution**:
    - Run `get_pdf.exe` to initiate automated extraction of the boot key:
     ```cmd
     .\get_pdf.exe 1 # if Azerty Keyboard
     .\get_pdf.exe 2 #  if Qwerty keyboard
     ```

   - Run get_bootkey.py to initiate automated extraction of the boot key:
     ```bash
     python3 get_bootkey.py
     ```

4. **Output**:
   - The script generates output detailing the extracted boot key.

## Development Status
This project is currently in development to provide a robust solution for automating LSA secret extraction from PDF documents. Feedback, contributions, and suggestions are welcome to enhance functionality and usability.

## Acknowledgments
- Aurélien CHALOT for their insightful article [Dumping LSA Secrets: A Story About Task Decorrelation](https://sensepost.com/blog/2024/dumping-lsa-secrets-a-story-about-task-decorrelation/), which inspired this project.
