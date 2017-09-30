#!/bin/bash

bin="../bin"
tiledir="${bin}/tiles"
datadir="../data"

methods=('bfs' 'dfs' 'greedy' 'astar')
mazes=('mediumMaze' 'bigMaze' 'openMaze')

for planner in ${methods[@]};
do
	out_dir="${datadir}/${planner}"
	mkdir -p ${out_dir}
	for maze in ${mazes[@]};
	do
		echo "Starting data collection for $maze using $planner..."
		${bin}/./mp_exec -maze "${bin}/${maze}.txt" \
			-agent_t "${tiledir}/vader.png" \
			-wall_t "${tiledir}/background.png"\
			-back_t "${tiledir}/obstacle.png"\
			-goal_t "${tiledir}/goal.png"\
			-out_gif "${out_dir}/${planner}_${maze}.gif"\
			-out "${out_dir}/${planner}_${maze}_path.txt"\
			-stats "${out_dir}/${planner}_${maze}_stats.txt"\
			-p $planner -h mdist
		echo "... Complete."
	done
done
