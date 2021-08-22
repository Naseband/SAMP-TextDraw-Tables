#include <a_samp>
#include <TDTables>
#include <zcmd>

#define MAX_ROWS 			25

new g_PlayerNames[10][] = { "Naseband", "TestPlayer1", "WWWWWWWWWWWWWWWW", "Testeysd", "[GSF]Loser", "TestPlayer1337", "Hii", "Eight", "Nine", "Ten" },
	g_Gangs[][] = { "Grove Street Boyz", "Ballas", "Epic Gamerz 1337", "-" };

// Constructor

new TDTable:g_MyTable<MAX_ROWS, 7>;

public OnFilterScriptInit()
{
	new const Float:widths[7] = { 120.0, 25.0, 25.0, 30.0, 25.0, 35.0, 70.0 };

	// Set Style

	TDT_SetPosition(g_MyTable, 320.0, 80.0);
	TDT_SetColumnWidths(g_MyTable, widths);

	// Create TDs

	TDT_Create(g_MyTable);

	printf("Total TDs: %d", TDT_CountTextDraws(g_MyTable));

	// Set Texts

	TDT_SetTitleText(g_MyTable, "s", "Gang War Results");
	TDT_SetHeaderRowText(g_MyTable, "sssssss", "Player Name", "Score", "Kills", "Deaths", "K/D", "Streak", "Gang");

	new num_rows, num_columns;
	TDT_GetSize(g_MyTable, num_rows, num_columns);

	for(new i = 0; i < num_rows; ++i)
	{
		new kills = 1 + random(100),
			deaths = 1 + random(50),
			Float:kd = float(kills) / float(deaths),
			streak = 1 + random(kills),
			score = 2*kills - deaths + streak;

		TDT_SetContentRowText(g_MyTable, i, "siiifis", g_PlayerNames[i % sizeof(g_PlayerNames)], score, kills, deaths, kd, streak, g_Gangs[random(sizeof(g_Gangs))]);
	}
}

public OnFilterScriptExit()
{
	TDT_Destroy(g_MyTable);
}

CMD:tdts(playerid, const params[])
{
	TDT_ShowForPlayer(g_MyTable, playerid);

	return 1;
}

CMD:tdth(playerid, const params[])
{
	TDT_HideForPlayer(g_MyTable, playerid);
	
	return 1;
}
