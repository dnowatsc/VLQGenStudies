#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms
import sys
import os

process = cms.Process("LHE")

#path = '/afs/cern.ch/work/d/dnowatsc/madgraph_tptp_production/'
path = '/afs/cern.ch/user/d/dnowatsc/TpTp_production/pairVLQ/'

processname=sys.argv[3]
name=sys.argv[4]

if not sys.argv[2].endswith('.lhe'):
    dirname=sys.argv[2]
    file_names=cms.untracked.vstring('file:'+path+dirname+'/'+processname+'/Events/'+name+'/unweighted_events.lhe')
else:
    file_names=cms.untracked.vstring('file:'+sys.argv[2])

if not os.path.exists(processname):
    os.mkdir(processname)

process.source = cms.Source("LHESource",
	fileNames = file_names
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.configurationMetadata = cms.untracked.PSet(
	version = cms.untracked.string('alpha'),
	name = cms.untracked.string('LHEF input'),
	annotation = cms.untracked.string('ttbar')
)

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cout = cms.untracked.PSet( threshold = cms.untracked.string('INFO') )

process.LHE = cms.OutputModule("PoolOutputModule",
	dataset = cms.untracked.PSet(dataTier = cms.untracked.string('LHE')),
	fileName = cms.untracked.string(processname+'/'+name+'_LHE.root')
)

process.lhedump = cms.EDAnalyzer("DummyLHEAnalyzer",
                                src = cms.InputTag("source")
                                )

process.TFileService=cms.Service("TFileService",fileName=cms.string(processname+'/'+name+'_HIST.root'))

process.outpath = cms.EndPath(process.LHE+process.lhedump)
