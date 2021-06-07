int themebutton(int id, double x, double y, double w, double h, char *label)
{    
	char * frameColor = gs_button_color.frame;
	char * labelColor = gs_button_color.label;
	double movement = 0.2*h;
	double shrink = 0.15*h;
	double sinkx = 0, sinky = 0;
	//int isHotItem = 0;

	if (notInMenu(gs_UIState.mousex, gs_UIState.mousey) &&
		inBox(gs_UIState.mousex, gs_UIState.mousey, x, x + w, y, y + h)) 
	{
		static int timesss = 0; timesss++;
		printf("%d not in %f %f %f %f\n", timesss, gs_menuRect[0], gs_menuRect[1], gs_menuRect[2], gs_menuRect[3]);
		frameColor = gs_button_color.hotFrame;
		labelColor = gs_button_color.hotLabel;
		gs_UIState.actingMenu = 0; // menu lose focus
		if ( gs_UIState.mousedown) {
			gs_UIState.clickedItem = id;
			sinkx =   movement;
			sinky = - movement;
		}
	}
	else {
		if ( gs_UIState.clickedItem==id )
			gs_UIState.clickedItem = 0;
	}

	// If no widget has keyboard focus, take it
	if (gs_UIState.kbdItem == 0)
		gs_UIState.kbdItem = id;
	// If we have keyboard focus, we'll need to process the keys
	if ( gs_UIState.kbdItem == id && gs_UIState.keyPress==VK_TAB ) 
	{
		// If tab is pressed, lose keyboard focus.
		// Next widget will grab the focus.
		gs_UIState.kbdItem = 0;
		// If shift was also pressed, we want to move focus
		// to the previous widget instead.
		if ( gs_UIState.keyModifiers & KMOD_SHIFT )
			gs_UIState.kbdItem = gs_UIState.lastItem;
		gs_UIState.keyPress = 0;
	}
	gs_UIState.lastItem = id;

	// draw the button
	mySetPenColor(frameColor);
	drawBox(x+sinkx, y+sinky, w, h, gs_button_color.fillflag,
		label, 'C', labelColor);
	if( gs_button_color.fillflag ) {
		mySetPenColor( labelColor );
		drawRectangle(x+sinkx, y+sinky, w, h, 0);
	}

	// ª≠º¸≈ÃÃ· æ, show a small ractangle frane
	if( gs_UIState.kbdItem == id ) {
		mySetPenColor( labelColor );
		drawRectangle(x+sinkx+shrink, y+sinky+shrink, w-2*shrink, h-2*shrink, 0);
	}

	if( gs_UIState.clickedItem==id && // must be clicked before
		! gs_UIState.mousedown )   // but now mouse button is up
	{
		gs_UIState.clickedItem = 0;
		gs_UIState.kbdItem = id;
		return 1; 
	}

	return 0;
}
