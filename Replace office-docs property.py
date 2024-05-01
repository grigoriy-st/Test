''' Скрипт для замены свойств office-документов'''
import docx, pptx, openpyxl, xlrd, xlwt
from xlutils.copy import copy

import os
current_user = input("Введите ФИО:")
root_path = os.path.abspath(os.path.dirname(__file__))
print(root_path)
root_path = 'D:/Practica'

for root, dirs, files in os.walk(root_path):
    for name in files:
        # print(os.path.join(name))
        filename, file_extension = os.path.splitext(name)
        if file_extension == '.docx':
            doc = docx.Document(os.path.join(root, name))
            if doc.core_properties.author != current_user:
                doc.core_properties.author = current_user
                doc.save(os.path.join(root, name))
        if file_extension == '.xlsx':
            wb = openpyxl.load_workbook(os.path.join(root, name))

            print(wb.properties.creator)
            wb.properties.creator = current_user
            wb.save(os.path.join(root, name))
            exit()
            

  
