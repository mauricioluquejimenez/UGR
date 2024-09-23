import pandas as pd
import numpy as np

from mplsoccer import Pitch, Sbopen
from mplsoccer import VerticalPitch,Pitch
from matplotlib import pyplot as plt
import networkx as nx

parser = Sbopen()
df, related, freeze, tactics = parser.event(18243)

df = df[df.type_name == 'Pass']
df = df[['x', 'y', 'end_x', 'end_y', "player_name", "pass_recipient_name","team_name","minute",'player_id']]
tactics = tactics[['jersey_number','player_id']]

# Merge tactics and df on player_id
df_merged = pd.merge(df, tactics[['jersey_number', 'player_id']], on='player_id', how='left')

# Define the name replacement dictionary
name_replacements = {  
    'Francisco Román Alarcón Suárez': 'Isco',
    'Sergio Ramos García': 'Sergio Ramos',
    'Cristiano Ronaldo dos Santos Aveiro': 'Cristiano Ronaldo',
    'Carlos Henrique Casimiro': 'Casemiro',
    'Marcelo Vieira da Silva Júnior': 'Marcelo',
    'Keylor Navas Gamboa': 'Keylor Navas',
    'Daniel Carvajal Ramos': 'Daniel Carvajal',
    'Gareth Frank Bale': 'Gareth Bale',
    'Kléper Laveran Lima Ferreira': 'Pepe',
    'Danilo Luiz da Silva': 'Danilo',
    'Lucas Vázquez Iglesias': 'Lucas Vázquez'
}

# Replace the names in the dataframe columns using the dictionary
df_merged['player_name'] = df_merged['player_name'].replace(name_replacements)
df_merged['pass_recipient_name'] = df_merged['pass_recipient_name'].replace(name_replacements)
df_merged.sort_values(by='team_name', inplace=True)

hteam = df_merged['team_name'].iloc[-1]
ateam = df_merged['team_name'].iloc[0]
print('Home Team : ' + hteam)
print('Away Team : ' + ateam)

df_away_pass = df_merged[df_merged.team_name == hteam]
print(df_away_pass)

# Check player play time
away_player_df = df_merged[df_merged.team_name == hteam].groupby('player_name').agg({'minute': ["min", "max"]}).reset_index()
away_player_df = pd.concat([away_player_df['player_name'], away_player_df['minute']], axis=1)
away_player_df['minutes_played'] = away_player_df['max'] - away_player_df['min']
away_player_df = away_player_df.sort_values('minutes_played', ascending=False)

away_player_name = away_player_df.player_name[:11].tolist()
df_away_pass = df_away_pass[df_away_pass.player_name.isin(away_player_name)]
df_away_pass = df_away_pass[df_away_pass.pass_recipient_name.isin(away_player_name)]

scatter_df = pd.DataFrame()
for i, name in enumerate(df_away_pass["player_name"].unique()):
    passx = df_away_pass.loc[df_away_pass["player_name"] == name]["x"].to_numpy()
    recx = df_away_pass.loc[df_away_pass["pass_recipient_name"] == name]["end_x"].to_numpy()
    passy = df_away_pass.loc[df_away_pass["player_name"] == name]["y"].to_numpy()
    recy = df_away_pass.loc[df_away_pass["pass_recipient_name"] == name]["end_y"].to_numpy()
    scatter_df.at[i, "player_name"] = name

    #make sure that x and y location for each circle representing the player is the average of passes and receptions
    scatter_df.at[i, "x"] = np.mean(np.concatenate([passx, recx]))
    scatter_df.at[i, "y"] = np.mean(np.concatenate([passy, recy]))

    #calculate number of passes
    scatter_df.at[i, "no"] = df_away_pass.loc[df_away_pass["player_name"] == name].count().iloc[0]
    scatter_df.at[i, "jersey_number"] = df_away_pass.loc[df_away_pass["player_name"] == name]['jersey_number'].iloc[0]
    scatter_df['jersey_number'] = scatter_df['jersey_number'].astype(int)
    
#adjust the size of a circle so that the player who made more passes
scatter_df['marker_size'] = (scatter_df['no'] / scatter_df['no'].max() * 1500)

#Calculate edge width
df_away_pass["pair_key"] = df_away_pass.apply(lambda x: "_".join(sorted([x["player_name"], x["pass_recipient_name"]])), axis=1)
lines_df = df_away_pass.groupby(["pair_key"]).x.count().reset_index()
lines_df.rename({'x':'pass_count'}, axis='columns', inplace=True)
#setting a treshold
lines_df = lines_df[lines_df['pass_count']>5]

pitch = VerticalPitch(pitch_type='statsbomb', line_color='white',linewidth=1,goal_type='box')
fig, ax = pitch.grid(grid_height=0.9, title_height=0.06, axis=False,
                     endnote_height=0.04, title_space=0, endnote_space=0)

# Plot vertices
pitch.scatter(scatter_df.x, scatter_df.y, s=scatter_df.marker_size, color='#272822', edgecolors='#EDBB00', linewidth=3, alpha=1, ax=ax["pitch"], zorder = 3)

#Add Players Name and Jersey Numbers
for i, row in scatter_df.iterrows():
    pitch.annotate(row.player_name, xy=(row.x +6, row.y), c='white', va='center',
                   ha='center', size=6, weight = "bold", ax=ax["pitch"], zorder = 4, 
                   bbox=dict(facecolor='#272822', alpha=1, edgecolor='#272822', boxstyle='round,pad=0.4'))
    
for i, row in scatter_df.iterrows():
    pitch.annotate(row.jersey_number, xy=(row.x, row.y-0.1), c='white', va='center',
                   ha='center', size=8, weight = "bold", ax=ax["pitch"], zorder = 4)

#Plot edges
for i, row in lines_df.iterrows():
        player1 = row["pair_key"].split("_")[0]
        player2 = row['pair_key'].split("_")[1]
        #take the average location of players to plot a line between them
        player1_x = scatter_df.loc[scatter_df["player_name"] == player1]['x'].iloc[0]
        player1_y = scatter_df.loc[scatter_df["player_name"] == player1]['y'].iloc[0]
        player2_x = scatter_df.loc[scatter_df["player_name"] == player2]['x'].iloc[0]
        player2_y = scatter_df.loc[scatter_df["player_name"] == player2]['y'].iloc[0]
        num_passes = row["pass_count"]
        #adjust the line width so that the more passes, the wider the line
        line_width = (num_passes / lines_df['pass_count'].max() * 8)
        # adjust the alpha of the lines based on number of passes and set minimum alpha for a fewer pass
        alpha = max(num_passes / lines_df['pass_count'].max(), 0.2)
        alpha = max(alpha, 0.5)
        #plot lines on the pitch
        pitch.lines(player1_x, player1_y, player2_x, player2_y,
                        alpha=alpha, lw=line_width, zorder=2, color="#EDBB00", ax = ax["pitch"])

fig.text(s=hteam + " "+ "Passing Networks", x= 0.06, y= 0.95, fontsize=18,fontweight="bold")
fig.text(s='Final Champions League 2015/2016', x = 0.06, y=0.90, fontsize=12)
plt.show()

G = nx.Graph()
G = nx.from_pandas_edgelist(df_away_pass, 'player_name', 'pass_recipient_name', 'x')
nx.write_gexf(G, "RMATM2016UCL.gexf")