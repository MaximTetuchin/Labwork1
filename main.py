import numpy as np
from PIL import Image

# Параметры изображения
width = 2800  # ширина изображения
height = 4200  # высота изображения
raw_file = "Images/gaussIMG.raw"  # путь к RAW-файлу

# Чтение RAW-файла
with open(raw_file, "rb") as file:
    raw_data = file.read()

# Преобразование данных в массив numpy
image_array = np.frombuffer(raw_data, dtype=np.uint8)  # предполагается 8-битное изображение
image_array = image_array.reshape((height, width))  # преобразуем в двумерный массив

# Сохраняем изображение как PNG
image = Image.fromarray(image_array, mode="L")  # mode "L" для ч/б изображений
image.save("output.png")

# Отображаем изображение
image.show()
