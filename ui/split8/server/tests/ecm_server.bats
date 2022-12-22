#!/usr/bin/env ../../bats/test/libs/bats/bin/bats
load '../../bats/test/libs/bats-support/load'
load '../../bats/test/libs/bats-assert/load'

@test "bats test" {
  assert_success
}

@test "Server exists" {
  echo $PWD/../server/miracle_controller 
  run nohup $PWD/../server/miracle_controller &
  #[ "$status-eq 0 ]
  [ "$?" -eq 0 ]
  assert_success
}

@test "Help message from server" {
  
}

@test "Server start on port 9400" {
  
}

@test "Server responds to " {
  assert_success
}

#Sysinit
#GetFreq
#GetStats
#SetFreq
#GetPower
#SetPower
#GetSampleRate
#SetSampleRate
#GetRxFreq
#SetRxFreq
#GetRxSampleRate
#SetRxSampleRate
#GetRxGains
#SetRxGains
#Sysoff
#StartSCP
#PrepSCP
#ResetNow
#SendUpdates
#CheckUpdates