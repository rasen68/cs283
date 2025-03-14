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
	  stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

     # Expected output with all whitespace removed for easier matching
     expected_output="execvp:Nosuchfileordirectorycmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
	  echo "${stripped_output} ->"
	  echo "${expected_output}"


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
	  stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

     # Expected output with all whitespace removed for easier matching
     expected_output="execvp:Nosuchfileordirectorycmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
 	  echo "${stripped_output} ->"
	  echo "${expected_output}"

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
	  stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

     # Expected output with all whitespace removed for easier matching
     expected_output="helloworldcmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
	  echo "${stripped_output} ->"
	  echo "${expected_output}"

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
	  stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

     # Expected output with all whitespace removed for easier matching
     expected_output="cmdloopreturned-7"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
	  echo "${stripped_output} ->"
	  echo "${expected_output}"

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
	  stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

     # Expected output with all whitespace removed for easier matching
     expected_output="/tmpcmdloopreturned0"

     # These echo commands will help with debugging and will only print
     #if the test fails
     echo "Captured stdout:" 
     echo "Output: $output"
     echo "Exit Status: $status"
	  echo "${stripped_output} ->"
	  echo "${expected_output}"

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
	 stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

    # Expected output with all whitespace removed for easier matching
    expected_output="/usr/bin/whichcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
	 echo "${stripped_output} ->"
	 echo "${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "It handles quoted spaces" {
    run "./dsh" <<EOF                
   echo " hello     world     " 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
	 stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//local mode/}

    # Expected output with all whitespace removed for easier matching
    expected_output=" hello     world        cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} ->"
	 echo "${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "ls pipe" {
    run ./dsh <<EOF                
ls | wc
EOF

    # Assertions
    [ "$status" -eq 0 ]
}


@test "cat grep wc?" {
    run "./dsh" <<EOF                
cat student_tests.sh | grep jijijijijiji | wc
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
	 stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

    # Expected output with all whitespace removed for easier matching
    expected_output="1746cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
	 echo "${stripped_output} ->"
	 echo "${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}


@test "wc wc wc?" {
    run "./dsh" <<EOF                
wc | wc | wc
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
	 stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

    # Expected output with all whitespace removed for easier matching
    expected_output="1324cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
	 echo "${stripped_output} ->"
	 echo "${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}


@test "ls directory?" {
    run "./dsh" <<EOF                
ls " hey guys"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
	 stripped_output=${stripped_output//dsh4>/}
	 stripped_output=${stripped_output//localmode/}

    # Expected output with all whitespace removed for easier matching
    expected_output="bidihicmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
	 echo "${stripped_output} ->"
	 echo "${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}


