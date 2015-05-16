#!/usr/bin/env python

import varial.tools
import sys
import os

if len(sys.argv) > 1:
    outputname='/'+sys.argv[1]
else:
    outputname=''

legendnames = {
        'm400narrowLHhiggs_HIST.root' : 'M400_LH_thth',
        'm800narrowLHhiggs_HIST.root' : 'M800_LH_thth',
        'm2000narrowLHhiggs_HIST.root' : 'M2000_LH_thth',
    }

p = varial.tools.mk_rootfile_plotter(
    name='hist_output'+outputname,
    pattern='*_HIST.root',
    #legendnames=legendnames,
    plotter_factory=varial.tools.Plotter,
    combine_files=True
)

if __name__ == '__main__':
    p.run()
    varial.tools.WebCreator().run()
    current_dir = os.getcwd().split('/')[-1]
    os.system('rsync -rltv hist_output nowatsd@bastion.desy.de:~/www/vlq_production/'+current_dir)
