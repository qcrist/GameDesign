#ifdef NOTES
to make the console window not show up
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	idea for menus:
		just change all the callbacks:
			meathod in each c file that adjust the callbacks
			meathod for reset of GL stuff
#endif