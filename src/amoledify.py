from PIL import Image
import numpy as np

image = Image.open("images/238/greenblackcv.png")
m = np.array(image)

print(m.shape)
m[m[:, :, 1] < 20] = [0, 0, 0]

image = Image.fromarray(m)
image.save("final.png")