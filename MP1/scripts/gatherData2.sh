#!/bin/bash

bin="../bin"
tiledir="${bin}/tiles"
datadir="../data"

methods=('astar')
mazes=('sokoban1' 'sokoban2' 'sokoban3' 'sokoban4')
heuristic="nearest"

for planner in ${methods[@]};
do
	out_dir="${datadir}/${planner}"
	mkdir -p ${out_dir}
	for maze in ${mazes[@]};
	do
		echo "Starting data collection for $maze using $planner..."
		${bin}/./mp_exec -maze "${bin}/${maze}.txt" \
			-agent_t "${tiledir}/vader.png" \
			-wall_t "${tiledir}/obstacle.png"\
			-back_t "${tiledir}/background.png"\
			-goal_t "${tiledir}/goal.png"\
			-box_t "${tiledir}/box.png"\
			-out_gif "${out_dir}/${planner}_${heuristic}_${maze}.gif"\
			-out "${out_dir}/${planner}_${heuristic}_${maze}_path.txt"\
			-stats "${out_dir}/${planner}_${heuristic}_${maze}_stats.txt"\
			-p $planner -h $heuristic
		echo "... Complete."
	done
done
