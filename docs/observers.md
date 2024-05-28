# Observers {#observers}

Vulp comes with the generic (that is, not robot-specific) observers listed below.

## History observer {#history-observer}

The history observer reports higher-frequency signals from the spine as vectors of observations to lower-frequency agents. It handles floating-point and vector-valued observations. For instance, to report the left-knee torque readings on an Upkie, create a shared-pointer as follows and add it to the observer pipeline of the spine:

```cpp
auto left_knee_torque_history = std::make_shared<HistoryObserver<double> >(
    /* keys = */ std::vector<std::string>{"servo", "left_knee", "torque"},
    /* size = */ 10,
    /* default_value = */ std::numeric_limits<double>::quiet_NaN());
observer_pipeline.append_observer(left_knee_torque_history);
```

The same syntax can be used for a vector like the IMU linear acceleration:

```cpp
auto linear_acceleration_history =
    std::make_shared<HistoryObserver<Eigen::Vector3d> >(
        /* keys = */ std::vector<std::string>{"imu", "linear_acceleration"},
        /* size = */ 10,
        /* default_value = */ Eigen::Vector3d::Zero());
observer_pipeline.append_observer(linear_acceleration_history);
```

Check out the API reference for details: \ref vulp::observation::HistoryObserver.
