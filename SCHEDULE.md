# Schedule Page

## Collaborators
Suresh Kannaian

## GitHub Projects Page
The public repository for "User RAM PStore" page can be found [here](https://github.com/cu-ecen-aeld/final-project-sureshkannaian/blob/main/SCHEDULE.md).

## Project Overview Link
For detailed information about the project, please visit our [Project Overview Page](https://github.com/cu-ecen-aeld/final-project-sureshkannaian/blob/main/OVERVIEW.md).


## Sprint 1 Issues
- Issue 1 - Assigned to Suresh Kannaian - DoD: Build Raspberry SD Card Image having u-boot.
- Issue 2 - Assigned to Suresh Kannaian - DoD: Using u-boot TFTP Kernel Image and Device Tree, then Boot Raspberry


## Top-level Schedule Table
| Issue | Assignee | DoD | Status | Blockers |
|-------|----------|-----|--------|----------|
| Set up Raspberry Pi 4           | Suresh Kannaian| Raspberry Pi 4 configured with UART peripherals and connectivity to host. Got Adafruit TTL serial port to usb.| Done |  |
| Build Yocto image               | Suresh Kannaian| Yocto image built with required configurations and packages          | Done |  |
| Implement Persistent Message Store | Suresh Kannaian | Persistent storage mechanism developed for custom user messages  | Planned | Yocto image readiness |

| Week 1 Issues                    | Assignee    | DoD                                                                 | Status  | Blockers       |
|----------------------------------|-------------|----------------------------------------------------------------------|---------|----------------|
| Set up Raspberry Pi 4            | SureshK | Raspberry Pi 4 configured with UART peripherals and connectivity to host. Use Adafruit TTL serial port to usb| Done |  |
| Build Yocto image                | SureshK | Yocto image built with required configurations and packages          | Done |  |
| Enable u-boot with Yocto image   | SureshK | Yocto image built with required configurations and packages(https://github.com/cu-ecen-aeld/final-project-sureshkannaian/commit/2761eeb2b7275367e3d59da249a6e9751414167f)          | Done |  |
| Enable TFTP boot options with u-boot to boot Linux  | SureshK | Yocto image built with required configurations and packages(https://github.com/cu-ecen-aeld/final-project-sureshkannaian/commit/2761eeb2b7275367e3d59da249a6e9751414167f) | Done |  |
| Build skeleton kernel  module for week2  | SureshK | Kernel Image built with module(https://github.com/cu-ecen-aeld/final-project-sureshkannaian/commit/f7e0347096931ccea727da193b4607c380e6898c#diff-e4c762fd81ad291ca8a5dc9f5f5f05cba29995b4b31033aa452856799cb79508)            | Done |  |


| Week 2 Issues                    | Assignee    | DoD                                                                 | Status  | Blockers       |
|----------------------------------|-------------|----------------------------------------------------------------------|---------|----------------|
| Reserve memory with device tree | SureshK | Reserve memory with RAM and not allocated to Kernel    | Planned | Build skeleton kernel  module for week 1 |
| Implement write to Message Store | SureshK | Persistent storage mechanism developed for custom user messages    | Planned | Reserve memory with device tree |


| Week 3 Issues                    | Assignee    | DoD                                                                 | Status  | Blockers       |
|----------------------------------|-------------|----------------------------------------------------------------------|---------|----------------|
| Implement read from Message Store | SureshK | Persistent storage read developed for custom user messages    | Planned | Reserve memory with device tree |
| Implement sysfs interface | SureshK | sysfs interface available in userspace    | Planned | Reserve memory with device tree |