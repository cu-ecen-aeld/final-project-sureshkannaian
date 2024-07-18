# User RAM Persistent Store

## Overview

This repository contains Suresh Kannaian submission for final project User RAM Persistent Store using Yocto build system.

## Project Overview

For detailed information about the project, please visit our [Project Overview Page](https://github.com/cu-ecen-aeld/final-project-sureshkannaian/blob/main/OVERVIEW.md).

## Table of Contents

1. [Introduction](#introduction)
2. [Project Objectives](#project-objectives)
3. [Contact Information](#contact-information)

## Introduction

### User RAM Persistent Store

The User RAM Persistent Store project aims to develop a mechanism for storing custom user messages in a persistent manner, ensuring these messages survive system reboots. Inspired by the Linux kernel's pstore and ramoops features used for capturing kernel crash logs, this project extends similar capabilities to user-defined messages, providing a reliable way to persist important information across reboots for diagnostic and monitoring purposes. The key benefits include a lightweight implementation, user-space application integration, enhanced diagnostics, support for application watchdogs, and flexibility for various use cases.


## Project Objectives

1. **Persistent Storage of Custom Messages**:
   - Develop a mechanism to store user-defined messages in a reserved RAM area that persists across system reboots.

2. **Minimization of Flash Writing and Wear Leveling**:
   - Design the solution to minimize writing to flash memory, reducing wear on flash storage devices commonly used in embedded systems.

3. **Enhanced Debugging and Maintenance**:
   - Enable the retention of logs across reboots, providing crucial diagnostic information for debugging and maintenance tasks in embedded environments.

4. **Integration with System Watchdogs**:
   - Support integration with system watchdogs to log critical events and states, ensuring continuous monitoring and reliability.

5. **Efficient Resource Utilization**:
   - Optimize resource usage to operate efficiently within the constraints of embedded systems, such as limited RAM and processing power.

6. **Flexible Configuration Options**:
   - Offer configurable parameters for memory allocation, record sizes, and storage policies to accommodate diverse embedded system requirements.

7. **Compatibility with Embedded Platforms**:
   - Ensure compatibility with various embedded platforms and operating systems, facilitating easy integration and deployment across different environments.

## Contact Information

For any questions or further information, please contact at [Suresh Kannaian](suresh.kannaian@colorado.edu).
