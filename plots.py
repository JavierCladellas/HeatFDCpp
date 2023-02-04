import matplotlib.pyplot as plt
import pandas as pd

#_____Export des solutions stationnaires_____#
#----CSV----
df = pd.read_csv("2Dsolutions/statio.csv")
fig, ax = plt.subplots()
plt.suptitle("Modèle stationnaire")
ax.set_xlabel("x (m)")
ax.set_ylabel("Température (°C)")
ax.plot(df["x"],df["temperature_simulee"],label="Simulée",marker=".",c="b")
ax.plot(df["x"],df["temperature_exacte"],label="Exacte",marker=",",c="r")
ax.legend()
plt.savefig("images/statio.png")


#_____Export des solutions stationnaires_____#
#---Scénario 1---#
df = pd.read_csv("2Dsolutions/instatio_1.csv")
fig, ax = plt.subplots()
plt.suptitle("Modèle instationnaire (Flux de chaleur constant)")
ax.set_xlabel("x (m)")
ax.set_ylabel("Température (°C)")
ax.plot(df[df["t"] == 15]["x"],df[df["t"] == 15]["temperature_simulee"],label="15s",c="r")
ax.plot(df[df["t"] == 30]["x"],df[df["t"] == 30]["temperature_simulee"],label="30s",c="b")
ax.plot(df[df["t"] == 60]["x"],df[df["t"] == 60]["temperature_simulee"],label="60s",c="k")
ax.plot(df[df["t"] == 90]["x"],df[df["t"] == 90]["temperature_simulee"],label="90s",c="y")
ax.plot(df[df["t"] == 150]["x"],df[df["t"] == 150]["temperature_simulee"],label="150s",c="g")
ax.plot(df[df["t"] == 210]["x"],df[df["t"] == 210]["temperature_simulee"],label="210s",c="c")
ax.legend()
plt.savefig("images/instatio_1.png")


#---Scénario 2---#
df = pd.read_csv("2Dsolutions/instatio_2.csv")
fig, ax = plt.subplots()
plt.suptitle("Modèle instationnaire (Activation/désactivation du flux de chaleur)")
ax.set_xlabel("Time (s)")
ax.set_ylabel("Température (°C)")
ax.plot(df["t"],df["x_0"],label=r"$x_0$",c="b")
ax.plot(df["t"],df["x_M/2"],label=r"$x_{M/2}$",c="r")
ax.plot(df["t"],df["x_M"],label=r"$x_M$",c="k")
ax.legend()
plt.savefig("images/instatio_2.png")

