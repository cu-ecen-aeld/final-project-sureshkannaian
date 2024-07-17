# Project Overview

## Project Goals and Motivation
## User RAM Persistent Store

The User RAM Persistent Store project aims to develop a mechanism for storing custom user messages in a persistent manner, ensuring these messages survive system reboots. Inspired by the Linux kernel's pstore and ramoops features used for capturing kernel crash logs, this project extends similar capabilities to user-defined messages, providing a reliable way to persist important information across reboots for diagnostic and monitoring purposes. The key benefits include a lightweight implementation, user-space application integration, enhanced diagnostics, support for application watchdogs, and flexibility for various use cases.

#### Differences from RAM Oops

- **Purpose**:
  - **RAM Oops**: Captures kernel crash logs, panic messages, and other critical kernel diagnostics.
  - **User RAM Persistent Store**: Stores custom user-defined messages, such as application logs and custom alerts.

- **Target Audience**:
  - **RAM Oops**: Kernel developers and system administrators.
  - **User RAM Persistent Store**: Application developers, system integrators, and users needing to persist custom messages.

- **Message Types**:
  - **RAM Oops**: Kernel logs, panic messages, and hardware error logs.
  - **User RAM Persistent Store**: Application logs, custom alerts, user-defined events, and watchdog messages.

- **Integration**:
  - **RAM Oops**: Integrated with the kernel’s crash reporting infrastructure.
  - **User RAM Persistent Store**: Provides APIs for user-space applications and custom kernel modules.

- **Configuration and Usage**:
  - **RAM Oops**: Suited for capturing critical system-level issues.
  - **User RAM Persistent Store**: Lightweight, easy to configure, and suitable for varied applications including user-space watchdogs.

User RAM Persistent Store
### RAM Oops Pstore Diagram

![User RAM Persistent Store Diagram](https://github.com/cu-ecen-aeld/final-project-sureshkannaian/blob/main/overview_image.png)

## Target Build System
The project uses Yocto.

## Hardware Platform
The hardware platform Raspberry PI 4 Model B.

## Open Source Projects
No open source projects/packages are used.

## Previous Assignment Content
No previous assignment content is used.

## Course Content
No previous course content is used.

## New Content
No previous course content is used.


## Source Code Organization
Outline the source code organization:
- Yocto implementation: [User-RAM-PStore](https://github.com/cu-ecen-aeld/final-project-sureshkannaian/tree/main/meta-aesd/recipes-user-ram-pstore)

## Team Project Members
List of team project members and roles:
- [Suresh Kannaina] - [Embedded Engineer]

## Shared Schedule Page
Link to the shared schedule page: [Schedule Page Link](https://github.com/cu-ecen-aeld/final-project-sureshkannaian/blob/main/SCHEDULE.md)
