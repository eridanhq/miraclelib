#!/usr/bin/env ../../bats/test/libs/bats/bin/bats
load '../../bats/test/libs/bats-support/load'
load '../../bats/test/libs/bats-assert/load'

@test "bats test" {
  assert_success
}

@test "Server exists" {
  [ -f miracle_controller ]
}

@test "Help message from server" {
  
}

@test "Server start on port 9400" {
  
}

@test "Server responds to " {
  
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