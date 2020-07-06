import numpy as np
import scipy.integrate
import matplotlib.pyplot as plt

BOX_VOLUME = 0.75*0.5*0.5  # in meters
BOX_SURFACE_AREA = 2*(0.5*0.75) + 2*(0.5*0.75) + 2*(0.5*0.5)
BOX_HEAT_TRANSFER = 3.4
HEAT_SOURCE_W = 200.0
FUME_PRODUCTION = 0.01  # m^3 of fume per second
START_TEMP = 30.0
START_FUMES = 0.0

AIR_SPECIFIC_HEAT = 1005.0
AIR_DENSITY = 1.225
BOX_AIR_MASS = AIR_DENSITY*BOX_VOLUME
T_AMBIENT = 25
RUNTIME = 60*10


def system_dynamics(t, y, u=0.0):
    """
    Simulates the thermal and toxic buildups over time as ODEs
    """
    ydot = np.array([0.0, 0.0])
    UA = (BOX_SURFACE_AREA*BOX_HEAT_TRANSFER)
    mCp = (AIR_SPECIFIC_HEAT*BOX_AIR_MASS)

    ydot[0] = UA/mCp*(T_AMBIENT - y[0]) + HEAT_SOURCE_W/mCp + u*((T_AMBIENT - y[0])/(BOX_VOLUME*mCp))
    ydot[1] = FUME_PRODUCTION - u*y[1]/BOX_VOLUME
    return ydot

def plot_vals(t, y):
    fig, ax1 = plt.subplots()

    color = 'tab:red'
    ax1.plot(t, y[0,:], color=color)

    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
    ax2.plot(t, y[1,:], color='tab:blue')

def no_control_sim():
    xs = np.linspace(0, RUNTIME, num=200)
    y0 = np.array([START_TEMP, START_FUMES])
    sol = scipy.integrate.solve_ivp(system_dynamics, [0, RUNTIME], y0, t_eval=xs)
    plot_vals(sol.t, sol.y)
    plt.show()

def constant_control_sim(uval):
    xs = np.linspace(0, RUNTIME, num=200)
    y0 = np.array([START_TEMP, START_FUMES])
    sol = scipy.integrate.solve_ivp(system_dynamics, [0, RUNTIME], y0, t_eval=xs, args=(uval,))
    plot_vals(sol.t, sol.y)
    plt.show()

def main():
    no_control_sim()
    constant_control_sim(0.2)


if __name__ == "__main__":
    main()