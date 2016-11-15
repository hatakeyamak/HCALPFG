
# example crab configuration file for one single run

##________________________________________________________________________________||
#Configurables

#dataset = '/JetHT/Run2015D-v1/RAW'
#run = '256729'
#dataset = '/MinimumBias/Commissioning2016-v1/RAW'
#dataset = '/JetHT/Run2016B-v2/RAW'
#run = '275311'

#dataset = '/JetHT/Run2016E-v2/RAW'
#run = '277305'

#dataset = '/JetHT/Run2016E-v2/RAW'
#run = '277420'

#dataset = '/JetHT/Run2016F-v1/RAW'
#run = '278406'
#run = '278018'
#run = '278240'

#dataset = '/JetHT/Run2016G-v1/RAW'
#run='278822'
#run='278873'
#run='278963'
#run='278969'
#run='278975'
#run='278986'

#run='279588'
#run='279479'

#run='278963-279667'

dataset = '/JetHT/Run2016H-v1/RAW'
#run='281638'
#run='281641'
#run='281693'
#run='281727'
#run='281797'
#run='282730-282735'
run='282922'

##________________________________________________________________________________||

jobname = dataset[1:].replace('/','__')
jobname = jobname.replace(':','___')
jobname = jobname.replace('RAW','RAW_'+run)

##________________________________________________________________________________||

from WMCore.Configuration import Configuration
config = Configuration()

##________________________________________________________________________________||

config.section_("General")
config.General.requestName  = jobname
#config.General.workArea     = 'out_crab'
#config.General.workArea     = 'run278406v3'
#config.General.workArea     = 'run277305'
#config.General.workArea     = 'run277420'
#config.General.workArea     = 'run278018'
config.General.workArea     = 'Run_282730to282735'

##________________________________________________________________________________||

config.section_("JobType")
config.JobType.pluginName   = 'Analysis'
#config.JobType.psetName     = 'analysis_Collision_crab_cfg.py'
#config.JobType.psetName     = 'pfg_CommissioningCosmic_RAW_cfg.py'
config.JobType.psetName     = 'pfg_Global_RAW_cfg.py'

##________________________________________________________________________________||

config.section_("Data")
config.Data.inputDataset    = dataset
config.Data.inputDBS        = 'global'
config.Data.splitting       = 'LumiBased'
config.Data.unitsPerJob     = 30 
#config.Data.publication     = True
config.Data.publication     = False
#config.Data.publishDBS      = 'phys03'
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.outputDatasetTag = 'Run_282730to282735'
config.Data.outLFNDirBase = '/store/user/borzou/ntuples/Run_282730to282735'
#config.Data.lumiMask        = 'json.txt'
config.Data.runRange        = run
config.Data.ignoreLocality  = True

##________________________________________________________________________________||

config.section_("Site")
#config.Site.storageSite     = 'T2_CH_CERN'
config.Site.storageSite     = 'T3_US_Baylor'

#config.Site.whitelist = ['T2_US_Caltech','T2_US_Florida', 'T2_US_MIT', 'T2_US_Nebraska', 'T2_US_Purdue', 'T2_US_UCSD', 'T2_US_Vanderbilt', 'T2_US_Wisconsin', 'T1_US_FNAL','T2_US_MIT']
# you may want to uncomment this line and force jobs to run in the US
# only a few datasets (mostly very new ones) will not be accessible
