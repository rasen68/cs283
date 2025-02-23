#!/usr/bin/env bats
@test "check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}
@test "unknown command" {
     run "./dsh" <<EOF                
	  a
EOF

     # Strip all whitespace (spaces, tabs, newlines) from the output
     stripped_output=$(echo "$output" | tr -d '[:space:]')

     # Expected output with all whitespace removed for easier matching
     expected_output="dsh2>error:couldntexecutecommanddsh2>dsh2>cmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
     echo "${stripped_output} -> ${expected_output}"

     # Check exact match
     [ "$stripped_output" = "$expected_output" ]

     # Assertions
     [ "$status" -eq 0 ]
 }

@test "unknown cd" {
     run "./dsh" <<EOF                
	  cd sdifjisdjfia
EOF

     # Strip all whitespace (spaces, tabs, newlines) from the output
     stripped_output=$(echo "$output" | tr -d '[:space:]')

     # Expected output with all whitespace removed for easier matching
     expected_output="dsh2>error:couldntexecutecommanddsh2>dsh2>cmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
     echo "${stripped_output} -> ${expected_output}"

     # Check exact match
     [ "$stripped_output" = "$expected_output" ]

     # Assertions
     [ "$status" -eq 0 ]
 }

@test "really long" {
     run "./dsh" <<EOF                
	  cdsdifjisdjfiajisjFIOSDHFISDHUFISUDHFIUSDHFIUSDFH
EOF

     # Strip all whitespace (spaces, tabs, newlines) from the output
     stripped_output=$(echo "$output" | tr -d '[:space:]')

     # Expected output with all whitespace removed for easier matching
     expected_output="dsh2>error:couldntexecutecommanddsh2>dsh2>cmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
     echo "${stripped_output} -> ${expected_output}"

     # Check exact match
     [ "$stripped_output" = "$expected_output" ]

     # Assertions
     [ "$status" -eq 0 ]
  } 

@test "echo hello world" {
     run "./dsh" <<EOF                
echo hello world
EOF

     # Strip all whitespace (spaces, tabs, newlines) from the output
     stripped_output=$(echo "$output" | tr -d '[:space:]')

     # Expected output with all whitespace removed for easier matching
     expected_output="helloworlddsh2>dsh2>cmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
     echo "${stripped_output} -> ${expected_output}"

     # Check exact match
     [ "$stripped_output" = "$expected_output" ]

     # Assertions
     [ "$status" -eq 0 ]
 }

@test "exit" {
     run "./dsh" <<EOF                
exit
EOF

     # Strip all whitespace (spaces, tabs, newlines) from the output
     stripped_output=$(echo "$output" | tr -d '[:space:]')

     # Expected output with all whitespace removed for easier matching
     expected_output="dsh2>cmdloopreturned-7"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
     echo "${stripped_output} -> ${expected_output}"

     # Check exact match
     [ "$stripped_output" = "$expected_output" ]

     # Assertions
     [ "$status" -eq 0 ]
 }


 @test "Change directory - no args" {
     current=$(pwd)

     cd /tmp
     mkdir -p dsh-test

     run "${current}/dsh" <<EOF                
 cd
 pwd
EOF

     # Strip all whitespace (spaces, tabs, newlines) from the output
     stripped_output=$(echo "$output" | tr -d '[:space:]')

     # Expected output with all whitespace removed for easier matching
     expected_output="/tmpdsh2>dsh2>dsh2>cmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
     echo "${stripped_output} -> ${expected_output}"

     # Check exact match
     [ "$stripped_output" = "$expected_output" ]

     # Assertions
     [ "$status" -eq 0 ]
 }


@test "Which which ... which?" {
    run "./dsh" <<EOF                
which which
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="/usr/bin/whichdsh2>dsh2>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "It handles quoted spaces" {
    run "./dsh" <<EOF                
   echo " hello     world     " 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output=" hello     world     dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}
