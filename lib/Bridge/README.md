# Bridge

> Acts as connector for any resources that travel between the 4 modules and UI. 

**Tasks:**
* Send notifications from UI to modules
* Receive requests from modules
* Send info to modules
* Host ISRs
* Act as entry point for async tasks
* Integrate UI code on a **shallow** level
* provide CLI

**Needs:**
* FreeRTOS
* Task handles
* ISRs
* Queues