#!/bin/bash

bin="../bin"
tiledir="${bin}/tiles"
datadir="../data"

methods=('astar')
mazes=('mediumSearch')
heuristic="nearest"

for planner in ${methods[@]};
do
	out_dir="${datadir}/${planner}"
	mkdir -p ${out_dir}
	for maze in ${mazes[@]};
	do
		file=${planner}_${heuristic}_${maze}
		echo "Starting data collection for $maze using $planner..."
		${bin}/./mp_exec -maze "${bin}/${maze}.txt" \
			-agent_t "${tiledir}/vader.png" \
			-wall_t "${tiledir}/obstacle.png"\
			-back_t "${tiledir}/background.png"\
			-goal_t "${tiledir}/goal.png"\
			-out_gif "${out_dir}/${file}.gif"\
			-out "${out_dir}/${file}_path.txt"\
			-stats "${out_dir}/${file}_stats.txt"\
			-p $planner -h $heuristic
		echo "... Complete."
	done
done
