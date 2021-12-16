#Setup
import numpy as np
import tensorflow as tf
from tensorflow import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout
#dropout : Randomly selected neurones are dropped out to prevent the model from overfitting
from keras.utils import np_utils
from tensorflow.keras import layers
import matplotlib.pyplot as plt
#src_path_train = "/home/hamza-elyousfi/Bureau/Implémentation d'une IA/data/train/"
import os
src_path_train = "bmpProcessed_png"


X_train = tf.keras.utils.image_dataset_from_directory(
  src_path_train,
  validation_split=0.08,
  subset="training",
  seed=123,
  image_size=(28, 28),
  color_mode='grayscale')

val_ds = tf.keras.utils.image_dataset_from_directory(
  src_path_train,
  validation_split=0.08,
  subset="validation",
  seed=123,
  image_size=(28, 28),
  color_mode ='grayscale')


class_names = X_train.class_names
print(class_names)



num_classes = len(class_names)


model = Sequential([
  layers.Rescaling(1./255, input_shape=(28, 28)),
  #data_augmentation,
  layers.Flatten(),
  layers.Dense(128, input_dim=28*28, activation='relu'),
  layers.Dense(num_classes)
])


model.compile(optimizer='adam',
              loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
              metrics=['accuracy'])

model.summary()

epochs=100
history = model.fit(
  X_train,
  validation_data=val_ds,
  batch_size = 10,
  epochs=epochs
)



acc = history.history['accuracy']
val_acc = history.history['val_accuracy']

loss = history.history['loss']
val_loss = history.history['val_loss']

epochs_range = range(epochs)

plt.figure(figsize=(8, 8))
plt.subplot(1, 2, 1)
plt.plot(epochs_range, acc, label='Training Accuracy')
plt.plot(epochs_range, val_acc, label='Validation Accuracy')
plt.legend(loc='lower right')
plt.title('Training and Validation Accuracy')

plt.subplot(1, 2, 2)
plt.plot(epochs_range, loss, label='Training Loss')
plt.plot(epochs_range, val_loss, label='Validation Loss')
plt.legend(loc='upper right')
plt.title('Training and Validation Loss')
plt.show()

weights = model.get_weights()

f=open("layer0_weights.txt", "w")
for i in range(784):
        for j in range(128):
                f.write(str(weights[0][i][j]))
                f.write(" ")
        f.write("\n")
f.close()

f=open("layer0_bias", "w")
for i in range(128):
        f.write(str(weights[1][i]))
        f.write(" ")
f.close()

f=open("layer1_weights.txt", "w")
for i in range(128):
        for j in range(10):
                f.write(str(weights[2][i][j]))
                f.write(" ")
        f.write("\n")
f.close()

f=open("layer1_bias", "w")
for i in range(10):
        f.write(str(weights[3][i]))
        f.write(" ")
f.close()

