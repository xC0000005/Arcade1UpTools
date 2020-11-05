from PIL import Image

def convert_to_png(source, target):
    raw_file = open(source, "rb")
    img = Image.new('RGBA', (640, 480))
    raw_bytes = bytearray(raw_file.read())
    byte_index = 0
    for y in range(0, 480):
        for x in range(0, 640):
            pixel_value = (raw_bytes[byte_index + 2], raw_bytes[byte_index + 1], raw_bytes[byte_index + 0],  255)#raw_bytes[byte_index + 3])
            img.putpixel((x, y), pixel_value)
            byte_index = byte_index + 4            
    img.save(target, "PNG")
    raw_file.close()

convert_to_png("01.565", "01.565.png")
convert_to_png("01.load", "01.load.png")

convert_to_png("02.565", "02.565.png")
convert_to_png("02.load", "02.load.png")

convert_to_png("03.565", "03.565.png")
convert_to_png("03.load", "03.load.png")

convert_to_png("b.565", "b.565.png")
convert_to_png("g.565", "g.565.png")
convert_to_png("r.565", "r.565.png")

convert_to_png("legal.565", "legal.png")
