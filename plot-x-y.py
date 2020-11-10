
import cv2
import numpy as np
import matplotlib.pyplot as plt

data_set = np.loadtxt(
    fname="tmp2",
    dtype="float",
    delimiter=",",
)

#散布図を描画 → scatterを使用する
#1行ずつ取り出して描画
#plt.scatter(x座標の値, y座標の値)
for data in data_set:
    plt.scatter(data[0], data[1], c='black')

plt.title("correlation")
#plt.xlabel("Average Temperature of SAITAMA")
#plt.ylabel("Average Temperature of IWATE")
#plt.grid()

#plt.savefig('tmp.png')
#test_image = cv2.imread("tmp.png", 0) #グレースケール画像として読み込む.
#plt.imshow(test_image)
plt.gray()
#plt.imshow(data_set,cmap='gray', vmin = 0, vmax = 255)
plt.show()
