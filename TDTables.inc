// ------------------------------------------------------------ 
/*

--- TDTables by NaS/Naseband
--- Highly customizable Text Draw Tables.

// ------------------------------------------------------------ 

Constructor:

--- new TDTable:Name<rows,columns>;

Style Functions:

--- TDT_Resize(TDTable:table, num_rows, num_columns)
--- TDT_GetSize(TDTable:table, &num_rows, &num_columns)
--- TDT_GetMaxSize(TDTable:table, &max_rows, &max_columns)
--- TDT_GetCurSize(TDTable:table, &cur_rows, &cur_columns)

--- TDT_SetPosition(TDTable:table, Float:x, Float:y)

--- TDT_SetBoxStyle(TDTable:table, box_color, box_alpha_offset)
--- TDT_SetTitleStyle(TDTable:table, font, Float:font_size, color_fg, color_bg)
--- TDT_SetHeaderStyle(TDTable:table, font, Float:font_size, color_fg, color_bg)
--- TDT_SetContentStyle(TDTable:table, font, Float:font_size, color_fg, color_bg)
--- TDT_SetSpacing(TDTable:table, Float:spacing)

--- TDT_SetColumnWidth(TDTable:table, column_id, Float:width)
--- TDT_SetColumnWidths(TDTable:table, Float:widths[], size = sizeof(widths))

Setup Functions:

--- TDT_Create(TDTable:table)
--- TDT_Destroy(TDTable:table)

--- TDT_SetTitleText(TDTable:table, const fmat[], {Float,bool,_}:...)

--- TDT_SetHeaderRowText(TDTable:table, const fmat[], {Float,bool,_}:...)
--- TDT_SetHeaderCellText(TDTable:table, column, const fmat[], {Float,bool,_}:...)

--- TDT_SetContentRowText(TDTable:table, row, const fmat[], {Float,bool,_}:...)
--- TDT_SetContentCellText(TDTable:table, row, column, const fmat[], {Float,bool,_}:...)

--- TDT_AddColumn(TDTable:table, Float:width, fmat[], ...)

Show/Hide Functions:

--- TDT_ShowForPlayer(TDTable:table, playerid)
--- TDT_HideForPlayer(TDTable:table, playerid)

Misc Functions:

// --- TDT_CountTextDraws(TDTable:table)

// ------------------------------------------------------------ 

Usage:

After declaring a table (which serves as a constructor), you must use the style functions.
Style functions do not have any effect after the table has been created (incl. size),
however it can be recreated at any time using TDT_Create().

The setup functions will create and destroy the actual TextDraws, as well as change the texts.
Content texts can only be changed while the TextDraws are created (shown or hidden) and will
be lost when it is destroyed. Title and header texts will persist.

Note:

This design only allocates the exact amount of columns and rows that are neccessary
for each table; similar to how foreach allocates its arrays.

*/
// ------------------------------------------------------------ 

#if defined __TD_TABLES_INC__
#endinput
#endif
#define __TD_TABLES_INC__

// ------------------------------------------------------------ Config (can be redefined before including)

#if !defined TDT_MAX_CONTENT_TEXT
#define TDT_MAX_CONTENT_TEXT 					64
#endif

#if !defined TDT_MAX_TITLE_TEXT
#define TDT_MAX_TITLE_TEXT 						64
#endif

#if !defined TDT_MAX_HEADER_TEXT
#define TDT_MAX_HEADER_TEXT 					64
#endif

#if !defined TDT_DEFAULT_POS
#define TDT_DEFAULT_POS 						320.0, 140.0
#endif

// Default Box Style

#if !defined TDT_DEFAULT_BOX_COLOR
#define TDT_DEFAULT_BOX_COLOR 					0x000000AA
#endif

#if !defined TDT_DEFAULT_BOX_ALPHA_OFFSET
#define TDT_DEFAULT_BOX_ALPHA_OFFSET 			0x30
#endif

// Default Title Style

#if !defined TDT_DEFAULT_TITLE_FONT
#define TDT_DEFAULT_TITLE_FONT 					0
#endif

#if !defined TDT_DEFAULT_TITLE_FONT_SIZE
#define TDT_DEFAULT_TITLE_FONT_SIZE				0.4
#endif

#if !defined TDT_DEFAULT_TITLE_COLOR_FG
#define TDT_DEFAULT_TITLE_COLOR_FG 				0x000000FF
#endif

#if !defined TDT_DEFAULT_TITLE_COLOR_BG
#define TDT_DEFAULT_TITLE_COLOR_BG 				0xFFFF88AA
#endif

// Default Header Style

#if !defined TDT_DEFAULT_HEADER_FONT
#define TDT_DEFAULT_HEADER_FONT 				1
#endif

#if !defined TDT_DEFAULT_HEADER_FONT_SIZE
#define TDT_DEFAULT_HEADER_FONT_SIZE 			0.25
#endif

#if !defined TDT_DEFAULT_HEADER_COLOR_FG
#define TDT_DEFAULT_HEADER_COLOR_FG 			0xFFFF88FF
#endif

#if !defined TDT_DEFAULT_HEADER_COLOR_BG
#define TDT_DEFAULT_HEADER_COLOR_BG 			0x000000FF
#endif

// Default Content Style

#if !defined TDT_DEFAULT_CONTENT_FONT
#define TDT_DEFAULT_CONTENT_FONT 				1
#endif

#if !defined TDT_DEFAULT_CONTENT_FONT_SIZE
#define TDT_DEFAULT_CONTENT_FONT_SIZE 			0.2
#endif

#if !defined TDT_DEFAULT_CONTENT_COLOR_FG
#define TDT_DEFAULT_CONTENT_COLOR_FG 			0xFFFFFFFF
#endif

#if !defined TDT_DEFAULT_CONTENT_COLOR_BG
#define TDT_DEFAULT_CONTENT_COLOR_BG 			0x000000FF
#endif

// Default Misc Style

#if !defined TDT_DEFAULT_SPACING
#define TDT_DEFAULT_SPACING 					4.0
#endif

// ------------------------------------------------------------ Definitions

#define TDT_FONT_SIZE_Y_MUL 					3.64 // 3.64
#define TDT_SPACING_Y_MUL 						34.0

#define TDT: __41c_

enum E_TDT_DATA
{
	bool:tdt_Created,
	tdt_NumRows, // Number of rows/cols for current data
	tdt_NumColumns,

	tdt_MaxRows, // Max number of rows/cols (size of arrays, cannot be changed obviously)
	tdt_MaxColumns,

	tdt_CurRows, // Number of rows/cols of currently created TDs - resizing does not take effect until re-creating
	tdt_CurColumns,

	Float:tdt_PosX,
	Float:tdt_PosY,

	// General Style

	bool:tdt_BoxSet,
	tdt_BoxColor,
	tdt_BoxAlphaOffset, // Box alternates between two alpha values if used

	// Title Style

	bool:tdt_TitleSet,
	tdt_TitleFont,
	Float:tdt_TitleFontSize,
	tdt_TitleColorFG,
	tdt_TitleColorBG,

	// Header Style

	bool:tdt_HeaderSet,
	tdt_HeaderFont,
	Float:tdt_HeaderFontSize,
	tdt_HeaderColorFG,
	tdt_HeaderColorBG,

	// Content Style

	bool:tdt_ContentSet,
	tdt_ContentFont,
	Float:tdt_ContentFontSize,
	tdt_ContentColorFG,
	tdt_ContentColorBG,

	// Misc Style

	bool:tdt_SpacingSet,
	Float:tdt_Spacing
};

// Constants are only available within this row, so all "dynamic" arrays must be outside of the enum

// --- new TDTable:Name<rows,columns>;
#define TDTable:%1<%2,%3> %1_Data[E_TDT_DATA]={false,%2,%3,%2,%3,%2,%3,TDT_DEFAULT_POS},Text:%1_Title,Text:%1_Header[%3],Text:%1_Content[%2][%3],Float:%1_ColumnWidths[%3],%1_TitleText[TDT_MAX_TITLE_TEXT],%1_HeaderTexts[%3][TDT_MAX_HEADER_TEXT]

// ------------------------------------------------------------ Style Functions

// --- TDT_Resize(TDTable:table, num_rows, num_columns)

#define TDT_Resize(%1,%2,%3) TDT:Resize(%1_Data, %2, %3)
stock TDT:Resize(data[E_TDT_DATA], num_rows, num_columns)
{
	if(	num_rows < 1 || num_rows > data[tdt_MaxRows] ||
		num_columns < 1 || num_columns > data[tdt_MaxColumns] )
		return 0;

	data[tdt_NumRows] = num_rows;
	data[tdt_NumColumns] = num_columns;

	return 1;
}

// --- TDT_GetSize(TDTable:table, &num_rows, &num_columns)

#define TDT_GetSize(%1,%2,%3) TDT:GetSize(%1_Data, %2, %3)
stock TDT:GetSize(const data[E_TDT_DATA], &num_rows, &num_columns)
{
	num_rows = data[tdt_NumRows];
	num_columns = data[tdt_NumColumns];

	return 1;
}

// --- TDT_GetMaxSize(TDTable:table, &max_rows, &max_columns)

#define TDT_GetMaxSize(%1,%2,%3) TDT:GetMaxSize(%1_Data, %2, %3)
stock TDT:GetMaxSize(const data[E_TDT_DATA], &max_rows, &max_columns)
{
	max_rows = data[tdt_MaxRows];
	max_columns = data[tdt_MaxColumns];

	return 1;
}

// --- TDT_GetCurSize(TDTable:table, &cur_rows, &cur_columns)

#define TDT_GetCurSize(%1,%2,%3) TDT:GetCurSize(%1_Data, %2, %3)
stock TDT:GetCurSize(const data[E_TDT_DATA], &cur_rows, &cur_columns)
{
	cur_rows = data[tdt_CurRows];
	cur_columns = data[tdt_CurColumns];

	return 1;
}

// --- TDT_SetPosition(TDTable:table, Float:x, Float:y)

#define TDT_SetPosition(%1,%2,%3) TDT:SetPosition(%1_Data, %2, %3)
stock TDT:SetPosition(data[E_TDT_DATA], Float:x, Float:y)
{
	data[tdt_PosX] = x;
	data[tdt_PosY] = y;

	return 1;
}

// --- TDT_SetBoxStyle(TDTable:table, box_color, box_alpha_offset = TDT_DEFAULT_BOX_ALPHA_OFFSET)

#define TDT_SetBoxStyle(%1,%2) TDT:SetBoxStyle(%1_Data, %2)
stock TDT:SetBoxStyle(data[E_TDT_DATA], box_color, box_alpha_offset = TDT_DEFAULT_BOX_ALPHA_OFFSET)
{
	data[tdt_BoxSet] = true;
	data[tdt_BoxColor] = box_color;
	data[tdt_BoxAlphaOffset] = box_alpha_offset;

	return 1;
}

// --- TDT_SetTitleStyle(TDTable:table, font, Float:font_size, color_fg, color_bg)

#define TDT_SetTitleStyle(%1,%2,%3,%4,%5) TDT:SetTitleStyle(%1_Data, %2, %3, %4, %5)
stock TDT:SetTitleStyle(data[E_TDT_DATA], font, Float:font_size, color_fg, color_bg)
{
	data[tdt_TitleSet] = true;
	data[tdt_TitleFont] = font;
	data[tdt_TitleFontSize] = font_size;
	data[tdt_TitleColorFG] = color_fg;
	data[tdt_TitleColorBG] = color_bg;

	return 1;
}

// --- TDT_SetHeaderStyle(TDTable:table, font, Float:font_size, color_fg, color_bg)

#define TDT_SetHeaderStyle(%1,%2,%3,%4,%5) TDT:SetHeaderStyle(%1_Data, %2, %3, %4, %5)
stock TDT:SetHeaderStyle(data[E_TDT_DATA], font, Float:font_size, color_fg, color_bg)
{
	data[tdt_HeaderSet] = true;
	data[tdt_HeaderFont] = font;
	data[tdt_HeaderFontSize] = font_size;
	data[tdt_HeaderColorFG] = color_fg;
	data[tdt_HeaderColorBG] = color_bg;

	return 1;
}

// --- TDT_SetContentStyle(TDTable:table, font, Float:font_size, color_fg, color_bg)

#define TDT_SetContentStyle(%1,%2,%3,%4,%5) TDT:SetContentStyle(%1_Data, %2, %3, %4, %5)
stock TDT:SetContentStyle(data[E_TDT_DATA], font, Float:font_size, color_fg, color_bg)
{
	data[tdt_ContentSet] = true;
	data[tdt_ContentFont] = font;
	data[tdt_ContentFontSize] = font_size;
	data[tdt_ContentColorFG] = color_fg;
	data[tdt_ContentColorBG] = color_bg;

	return 1;
}

// --- TDT_SetSpacing(TDTable:table, Float:spacing)

#define TDT_SetSpacing(%1,%2) TDT:SetSpacing(%1_Data, %2)
stock TDT:SetSpacing(data[E_TDT_DATA], Float:spacing)
{
	data[tdt_SpacingSet] = true;
	data[tdt_Spacing] = spacing;

	return 1;
}

// --- TDT_SetColumnWidth(TDTable:table, column_id, Float:width)

#define TDT_SetColumnWidth(%1,%2,%3) TDT:SetColumnWidth(%1_Data, %1_ColumnWidths, %2, %3)
stock TDT:SetColumnWidth(const data[E_TDT_DATA], Float:column_widths[], column_id, Float:width)
{
	new num_columns = data[tdt_MaxColumns];

	if(column_id < 0 || column_id >= num_columns)
		return 0;

	column_widths[column_id] = width;

	return 1;
}

// --- TDT_SetColumnWidths(TDTable:table, Float:widths[], size = sizeof(widths))

#define TDT_SetColumnWidths(%1,%2) TDT:SetColumnWidths(%1_Data, %1_ColumnWidths, %2)
stock TDT:SetColumnWidths(const data[E_TDT_DATA], Float:column_widths[], const Float:widths[], size = sizeof(widths))
{
	new num_columns = data[tdt_MaxColumns];

	for(new i = 0; i < num_columns && i < size; ++i)
	{
		column_widths[i] = widths[i];
	}

	return 1;
}

// --- TDT_ResetRows(TDTable:table)

#define TDT_ResetRows(%1) TDT:ResetRows(%1_Data)
stock TDT:ResetRows(data[E_TDT_DATA])
{
	data[tdt_NumRows] = 0;
	return 1;
}

#define TDT_ResetColumns(%1) TDT:ResetColumns(%1_Data)
stock TDT:ResetColumns(data[E_TDT_DATA])
{
	data[tdt_NumColumns] = 0;
	return 1;
}

// ------------------------------------------------------------ Setup Functions

// --- TDT_Create(TDTable:table)

#define TDT_Create(%1) TDT:Create(%1_Data, %1_Title, %1_Header, %1_Content, %1_ColumnWidths, %1_TitleText, %1_HeaderTexts)
stock TDT:Create(data[E_TDT_DATA], &Text:title, Text:header[], Text:content[][], const Float:column_widths[], const title_text[], const header_texts[][])
{
	new num_rows = data[tdt_NumRows],
		num_columns = data[tdt_NumColumns],
		Float:base_x = data[tdt_PosX],
		Float:base_y = data[tdt_PosY],
		Float:x,
		Float:y,
		Float:spacing,
		Float:total_width,
		Text:td;

	if(data[tdt_Created])
		TDT:Destroy(data, title, header, content);

	data[tdt_CurRows] = num_rows;
	data[tdt_CurColumns] = num_columns;

	if(!data[tdt_BoxSet])
	{
		// default box style

		data[tdt_BoxSet] = true;
		data[tdt_BoxColor] = TDT_DEFAULT_BOX_COLOR;
		data[tdt_BoxAlphaOffset] = TDT_DEFAULT_BOX_ALPHA_OFFSET;
	}

	if(!data[tdt_TitleSet])
	{
		// default title style

		data[tdt_TitleSet] = true;
		data[tdt_TitleFont] = TDT_DEFAULT_TITLE_FONT;
		data[tdt_TitleFontSize] = TDT_DEFAULT_TITLE_FONT_SIZE;
		data[tdt_TitleColorFG] = TDT_DEFAULT_TITLE_COLOR_FG;
		data[tdt_TitleColorBG] = TDT_DEFAULT_TITLE_COLOR_BG;
	}

	if(!data[tdt_HeaderSet])
	{
		// default header style

		data[tdt_HeaderSet] = true;
		data[tdt_HeaderFont] = TDT_DEFAULT_HEADER_FONT;
		data[tdt_HeaderFontSize] = TDT_DEFAULT_HEADER_FONT_SIZE;
		data[tdt_HeaderColorFG] = TDT_DEFAULT_HEADER_COLOR_FG;
		data[tdt_HeaderColorBG] = TDT_DEFAULT_HEADER_COLOR_BG;
	}

	if(!data[tdt_ContentSet])
	{
		// default content style

		data[tdt_ContentSet] = true;
		data[tdt_ContentFont] = TDT_DEFAULT_CONTENT_FONT;
		data[tdt_ContentFontSize] = TDT_DEFAULT_CONTENT_FONT_SIZE;
		data[tdt_ContentColorFG] = TDT_DEFAULT_CONTENT_COLOR_FG;
		data[tdt_ContentColorBG] = TDT_DEFAULT_CONTENT_COLOR_BG;
	}

	if(!data[tdt_SpacingSet])
	{
		// default spacing style

		data[tdt_SpacingSet] = true;
		data[tdt_Spacing] = TDT_DEFAULT_SPACING;
	}

	// Spacing

	spacing = data[tdt_Spacing];

	// Total width

	total_width = (num_columns - 1) * spacing;

	for(new c = 0; c < num_columns; ++c)
		total_width += column_widths[c];

	// Title

	td = TextDrawCreate(base_x, base_y, title_text);
	TextDrawFont(td, data[tdt_TitleFont]);
	TextDrawAlignment(td, 2);
	TextDrawColor(td, data[tdt_TitleColorFG]);
	TextDrawBackgroundColor(td, data[tdt_TitleColorBG]);
	TextDrawBoxColor(td, data[tdt_BoxColor]);
	TextDrawUseBox(td, 1);
	TextDrawSetOutline(td, 1);
	TextDrawSetShadow(td, 0);
	TextDrawLetterSize(td, data[tdt_TitleFontSize], data[tdt_TitleFontSize] * TDT_FONT_SIZE_Y_MUL);
	TextDrawSetProportional(td, 1);
	TextDrawTextSize(td, data[tdt_TitleFontSize] * TDT_SPACING_Y_MUL, total_width);
	title = td;

	// Header & Content

	x = base_x - (total_width / 2.0) /*- (spacing * (num_columns - 1) / 2.0)*/; // left most x minus half the spacers

	for(new c = 0; c < num_columns; ++c)
	{
		y = base_y + data[tdt_TitleFontSize] * TDT_SPACING_Y_MUL + spacing;

		td = TextDrawCreate(x + (column_widths[c] / 2.0), y, header_texts[c]);
		TextDrawFont(td, data[tdt_HeaderFont]);
		TextDrawAlignment(td, 2);
		TextDrawColor(td, data[tdt_HeaderColorFG]);
		TextDrawBackgroundColor(td, data[tdt_HeaderColorBG]);
		TextDrawBoxColor(td, data[tdt_BoxColor]);
		TextDrawUseBox(td, 1);
		TextDrawSetOutline(td, 1);
		TextDrawSetShadow(td, 0);
		TextDrawLetterSize(td, data[tdt_HeaderFontSize], data[tdt_HeaderFontSize] * TDT_FONT_SIZE_Y_MUL);
		TextDrawSetProportional(td, 1);
		TextDrawTextSize(td, data[tdt_HeaderFontSize] * TDT_SPACING_Y_MUL, column_widths[c]);
		header[c] = td;

		for(new r = 0; r < num_rows; ++r)
		{
			y = base_y + data[tdt_TitleFontSize] * TDT_SPACING_Y_MUL + spacing + data[tdt_HeaderFontSize] * TDT_SPACING_Y_MUL + spacing + (r * data[tdt_ContentFontSize] * TDT_SPACING_Y_MUL) + (r * spacing);

			td = TextDrawCreate(x + (column_widths[c] / 2.0), y, "content");
			TextDrawFont(td, data[tdt_ContentFont]);
			TextDrawAlignment(td, 2);
			TextDrawColor(td, data[tdt_ContentColorFG]);
			TextDrawBackgroundColor(td, data[tdt_ContentColorBG]);
			TextDrawBoxColor(td, data[tdt_BoxColor] - (r % 2 == 0 ? data[tdt_BoxAlphaOffset] : 0));
			TextDrawUseBox(td, 1);
			TextDrawSetOutline(td, 1);
			TextDrawSetShadow(td, 0);
			TextDrawLetterSize(td, data[tdt_ContentFontSize], data[tdt_ContentFontSize] * TDT_FONT_SIZE_Y_MUL);
			TextDrawSetProportional(td, 1);
			TextDrawTextSize(td, data[tdt_ContentFontSize] * TDT_SPACING_Y_MUL, column_widths[c]);
			content[r][c] = td;
		}

		x += (spacing + column_widths[c]);
	}

	data[tdt_Created] = true;

	return 1;
}

// --- TDT_Destroy(TDTable:table)

#define TDT_Destroy(%1) TDT:Destroy(%1_Data, %1_Title, %1_Header, %1_Content)
stock TDT:Destroy(data[E_TDT_DATA], Text:title, const Text:header[], const Text:content[][])
{
	if(!data[tdt_Created])
		return 0;

	new num_rows = data[tdt_CurRows],
		num_columns = data[tdt_CurColumns];

	TextDrawDestroy(title);

	for(new c = 0; c < num_columns; ++c)
	{
		TextDrawDestroy(header[c]);

		for(new r = 0; r < num_rows; ++r)
		{
			TextDrawDestroy(content[r][c]);
		}
	}

	data[tdt_Created] = false;

	return 1;
}

// --- TDT_SetTitleText(TDTable:table, const fmat[], {Float,bool,_}:...)
// --- Note: If va_format is defined (YSI/y_va) it is an actual string format like format(). Otherwise it will only take one specifier which is the type of the argument passed.

#define TDT_SetTitleText(%1,%2,%3) TDT:SetTitleText(%1_Data, %1_Title, %1_TitleText, %2, %3)
stock TDT:SetTitleText(const data[E_TDT_DATA], Text:title, title_text[], const fmat[], {Float,bool,_}:...)
{
	#define ARG_IDX 	4

#if defined va_format
	
	va_format(title_text, TDT_MAX_TITLE_TEXT, fmat, va_start<ARG_IDX>);

	if(data[tdt_Created])
		TextDrawSetString(title, title_text);

#else

	new num_fmat = strlen(fmat),
		num_passed = numargs() - ARG_IDX,
		ch, len;

	if(num_fmat != 1 || num_passed != 1)
	{
		printf("TDT_SetTitleText(): Specifier count does not match argument count (must be 1).");
		return 0;
	}

	switch(fmat[0])
	{
		case 'i','b','d':
		{
			format(title_text, TDT_MAX_TITLE_TEXT, "%d", getarg(ARG_IDX));
			
			if(data[tdt_Created])
				TextDrawSetString(title, title_text);
		}
		case 'f':
		{
			format(title_text, TDT_MAX_TITLE_TEXT, "%.02f", getarg(ARG_IDX));
			
			if(data[tdt_Created])
				TextDrawSetString(title, title_text);
		}
		case 's':
		{
			len = 0;

			while((ch = getarg(ARG_IDX, len)) && len < TDT_MAX_TITLE_TEXT - 1)
			{
				title_text[len++] = ch;
			}

			title_text[len] = 0;

			if(data[tdt_Created])
				TextDrawSetString(title, title_text);
		}
	}

#endif

	#undef ARG_IDX

	return 1;
}

// --- TDT_SetHeaderRowText(TDTable:table, const fmat[], {Float,bool,_}:...)
//	 Note: Not a format like format(), each specifier is one column!

#define TDT_SetHeaderRowText(%1,%2,%3) TDT:SetHeaderRowText(%1_Data, %1_Header, %1_HeaderTexts, %2, %3)
stock TDT:SetHeaderRowText(const data[E_TDT_DATA], const Text:header[], header_texts[][], const fmat[], {Float,bool,_}:...)
{
	#define ARG_IDX 	4

	new num_columns = data[tdt_MaxColumns],
		num_fmat = strlen(fmat),
		num_passed = numargs() - ARG_IDX,
		ch, len;

	if(num_fmat == 0 || num_fmat > num_columns)
		return 0;

	if(num_fmat != num_passed)
	{
		printf("TDT_SetHeaderRowText(): Specifier count does not match argument count (%d/%d).", num_fmat, num_passed);
		return 0;
	}

	for(new c = 0; c < num_fmat; ++c)
	{
		switch(fmat[c])
		{
			case 'i','b','d':
			{
				format(header_texts[c], TDT_MAX_HEADER_TEXT, "%d", getarg(ARG_IDX + c));
				
				if(data[tdt_Created] && c < data[tdt_CurColumns])
					TextDrawSetString(header[c], header_texts[c]);
			}
			case 'f':
			{
				format(header_texts[c], TDT_MAX_HEADER_TEXT, "%.02f", getarg(ARG_IDX + c));
				
				if(data[tdt_Created] && c < data[tdt_CurColumns])
					TextDrawSetString(header[c], header_texts[c]);
			}
			case 's':
			{
				len = 0;

				while((ch = getarg(ARG_IDX + c, len)) && len < TDT_MAX_HEADER_TEXT - 1)
				{
					header_texts[c][len++] = ch;
				}

				header_texts[c][len] = 0;

				if(data[tdt_Created] && c < data[tdt_CurColumns])
					TextDrawSetString(header[c], header_texts[c]);
			}
		}
	}

	return 1;

	#undef ARG_IDX
}

// --- TDT_SetHeaderCellText(TDTable:table, column, const fmat[], {Float,bool,_}:...)
//	 Note: If va_format is defined (YSI/y_va) it is an actual string format like format(). Otherwise it will only take one specifier which is the type of the argument passed.

#define TDT_SetHeaderCellText(%1,%2,%3,%4) TDT:SetHeaderCellText(%1_Data, %1_Header, %1_HeaderTexts, %2, %3, %4)
stock TDT:SetHeaderCellText(const data[E_TDT_DATA], const Text:header[], header_texts[][], column, const fmat[], {Float,bool,_}:...)
{
	#define ARG_IDX 	5

	new num_columns = data[tdt_MaxColumns];

	if(column < 0 || column >= num_columns)
		return 0;

#if defined va_format
	
	va_format(header_texts[column], sizeof(text), fmat, va_start<ARG_IDX>);

	if(data[tdt_Created] && column < data[tdt_CurColumns]) 
		TextDrawSetString(header[column], header_texts[column]);

#else

	new num_fmat = strlen(fmat),
		num_passed = numargs() - ARG_IDX,
		ch, len;

	if(num_fmat != 1 || num_passed != 1)
	{
		printf("TDT_SetHeaderCellText(): Specifier count does not match argument count (must be 1).");
		return 0;
	}

	switch(fmat[0])
	{
		case 'i','b','d':
		{
			format(header_texts[column], TDT_MAX_HEADER_TEXT, "%d", getarg(ARG_IDX));
			
			if(data[tdt_Created] && column < data[tdt_CurColumns])
				TextDrawSetString(header[column], header_texts[column]);
		}
		case 'f':
		{
			format(header_texts[column], TDT_MAX_HEADER_TEXT, "%.02f", getarg(ARG_IDX));
			
			if(data[tdt_Created] && column < data[tdt_CurColumns])
				TextDrawSetString(header[column], header_texts[column]);
		}
		case 's':
		{
			len = 0;

			while((ch = getarg(ARG_IDX, len)) && len < TDT_MAX_HEADER_TEXT - 1)
			{
				header_texts[column][len++] = ch;
			}

			header_texts[column][len] = 0;

			if(data[tdt_Created] && column < data[tdt_CurColumns])
				TextDrawSetString(header[column], header_texts[column]);
		}
	}

#endif

	#undef ARG_IDX

	return 1;
}

// --- TDT_SetContentRowText(TDTable:table, row, const fmat[], {Float,bool,_}:...)
//	 Note: Not a format like format(), each specifier is one column!

#define TDT_SetContentRowText(%1,%2,%3,%4) TDT:SetContentRowText(%1_Data, %1_Content, %2, %3, %4)
stock TDT:SetContentRowText(const data[E_TDT_DATA], const Text:content[][], row, const fmat[], {Float,bool,_}:...)
{
	#define ARG_IDX 	4

	new num_rows = data[tdt_CurRows],
		num_columns = data[tdt_CurColumns],
		text[TDT_MAX_CONTENT_TEXT],
		num_fmat = strlen(fmat),
		num_passed = numargs() - ARG_IDX,
		ch, len;

	if(!data[tdt_Created] || row < 0 || row >= num_rows || num_fmat == 0 || num_fmat > num_columns)
		return 0;

	if(num_fmat != num_passed)
	{
		printf("TDT_SetContentRowText(): Specifier count does not match argument count (%d/%d).", num_fmat, num_passed);
		return 0;
	}

	for(new c = 0; c < num_fmat; ++c)
	{
		switch(fmat[c])
		{
			case 'i','b','d':
			{
				format(text, sizeof(text), "%d", getarg(ARG_IDX + c));
				
				TextDrawSetString(content[row][c], text);
			}
			case 'f':
			{
				format(text, sizeof(text), "%.02f", getarg(ARG_IDX + c));
				
				TextDrawSetString(content[row][c], text);
			}
			case 's':
			{
				len = 0;

				while((ch = getarg(ARG_IDX + c, len)) && len < sizeof(text) - 1)
				{
					text[len++] = ch;
				}

				text[len] = 0;

				TextDrawSetString(content[row][c], text);
			}
		}
	}

	return 1;

	#undef ARG_IDX
}

// --- TDT_SetContentCellText(TDTable:table, row, column, const fmat[], {Float,bool,_}:...)
//	 Note: If va_format is defined (YSI/y_va) it is an actual string format like format(). Otherwise it will only take one specifier which is the type of the argument passed.

#define TDT_SetContentCellText(%1,%2,%3,%4,%5) TDT:SetContentCellText(%1_Data, %1_Content, %2, %3, %4, %5)
stock TDT:SetContentCellText(const data[E_TDT_DATA], const Text:content[][], row, column, const fmat[], {Float,bool,_}:...)
{
	#define ARG_IDX 	5

	new num_rows = data[tdt_CurRows],
		num_columns = data[tdt_CurColumns],
		text[TDT_MAX_CONTENT_TEXT];

	if(!data[tdt_Created] || row < 0 || row >= num_rows || column < 0 || column >= num_columns)
		return 0;

#if defined va_format
	
	va_format(text, sizeof(text), fmat, va_start<ARG_IDX>);

	TextDrawSetString(content[row][column], text);

#else

	new num_fmat = strlen(fmat),
		num_passed = numargs() - ARG_IDX,
		ch, len;

	if(num_fmat != 1 || num_passed != 1)
	{
		printf("TDT_SetContentCellText(): Specifier count does not match argument count (must be 1).");
		return 0;
	}

	switch(fmat[0])
	{
		case 'i','b','d':
		{
			format(text, sizeof(text), "%d", getarg(ARG_IDX));
			
			TextDrawSetString(content[row][column], text);
		}
		case 'f':
		{
			format(text, sizeof(text), "%.02f", getarg(ARG_IDX));
			
			TextDrawSetString(content[row][column], text);
		}
		case 's':
		{
			len = 0;

			while((ch = getarg(ARG_IDX, len)) && len < sizeof(text) - 1)
			{
				text[len++] = ch;
			}

			text[len] = 0;

			TextDrawSetString(content[row][column], text);
		}
	}

#endif

	#undef ARG_IDX

	return 1;
}

// --- TDT_AddColumn(TDTable:table, Float:width, fmat[], ...) // Adds a new column with given width and text - use TDT_ResetColumns if you want to rebuild the table

#define TDT_AddColumn(%1,%2,%3,%4) TDT:AddColumn(%1_Data, %1_ColumnWidths, %1_HeaderTexts, %2, %3, %4)
stock TDT:AddColumn(data[E_TDT_DATA], Float:column_widths[], header_texts[][], Float:width, const fmat[], {Float,bool,_}:...)
{
	#define ARG_IDX 	5

	new max_columns = data[tdt_MaxColumns],
		column = data[tdt_NumColumns];

	if(column < 0 || column >= max_columns)
		return 0;

	data[tdt_NumColumns] = data[tdt_NumColumns] + 1; // ++ does not let the compiler know we modify data[], thus throwing a const warning
	column_widths[column] = width;

#if defined va_format
	
	va_format(header_texts[column], sizeof(text), fmat, va_start<ARG_IDX>);

#else

	new num_fmat = strlen(fmat),
		num_passed = numargs() - ARG_IDX,
		ch, len;

	if(num_fmat != 1 || num_passed != 1)
	{
		printf("TDT_AddColumn(): Specifier count does not match argument count (must be 1).");
		return 0;
	}

	switch(fmat[0])
	{
		case 'i','b','d':
		{
			format(header_texts[column], TDT_MAX_HEADER_TEXT, "%d", getarg(ARG_IDX));
		}
		case 'f':
		{
			format(header_texts[column], TDT_MAX_HEADER_TEXT, "%.02f", getarg(ARG_IDX));
		}
		case 's':
		{
			len = 0;

			while((ch = getarg(ARG_IDX, len)) && len < TDT_MAX_HEADER_TEXT - 1)
			{
				header_texts[column][len++] = ch;
			}

			header_texts[column][len] = 0;
		}
	}

#endif

	#undef ARG_IDX

	return 1;
}

// ------------------------------------------------------------ Show/Hide

// --- TDT_ShowForPlayer(TDTable:table, playerid)

#define TDT_ShowForPlayer(%1,%2) TDT:ShowForPlayer(%1_Data, %1_Title, %1_Header, %1_Content, %2)
stock TDT:ShowForPlayer(const data[E_TDT_DATA], Text:title, const Text:header[], const Text:content[][], playerid)
{
	if(!data[tdt_Created])
		return 0;

	new num_rows = data[tdt_CurRows],
		num_columns = data[tdt_CurColumns];

	TextDrawShowForPlayer(playerid, title);

	for(new c = 0; c < num_columns; ++c)
	{
		TextDrawShowForPlayer(playerid, header[c]);

		for(new r = 0; r < num_rows; ++r)
		{
			TextDrawShowForPlayer(playerid, content[r][c]);
		}
	}

	return 1;
}

// --- TDT_HideForPlayer(TDTable:table, playerid)

#define TDT_HideForPlayer(%1,%2) TDT:HideForPlayer(%1_Data, %1_Title, %1_Header, %1_Content, %2)
stock TDT:HideForPlayer(const data[E_TDT_DATA], Text:title, const Text:header[], const Text:content[][], playerid)
{
	if(!data[tdt_Created])
		return 0;

	new num_rows = data[tdt_CurRows],
		num_columns = data[tdt_CurColumns];

	TextDrawHideForPlayer(playerid, title);

	for(new c = 0; c < num_columns; ++c)
	{
		TextDrawHideForPlayer(playerid, header[c]);

		for(new r = 0; r < num_rows; ++r)
		{
			TextDrawHideForPlayer(playerid, content[r][c]);
		}
	}

	return 1;
}

// ------------------------------------------------------------ Misc

// --- TDT_CountTextDraws(TDTable:table)

#define TDT_CountTextDraws(%1) TDT:CountTextDraws(%1_Data)
stock TDT:CountTextDraws(const data[E_TDT_DATA])
{
	if(!data[tdt_Created])
		return 0;

	return 1 + (data[tdt_CurRows] + 1) * data[tdt_CurColumns];
}

// ------------------------------------------------------------ EOF
