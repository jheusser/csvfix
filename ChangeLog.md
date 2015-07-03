<h3>The current version of CSVfix is 1.5 </h3>


&lt;hr&gt;


<b>
Changes from Version 1.4 to 1.5<br>
</b>

Added  -skip and -pass options to filter command input and output.

Added ability to specify default options and aliases via configuration files.

Actual config file in use is displayed if CSVfix is run with no command line parameters.

Added head and tail commands to display first/last CSV records in file.

Added -ic option to join command to ignore case of joined fields.

Added -f option to printf command to specify order.

Added -s option to asci\_table command to add separator after each record.

The template command now allows expressions in templates.

Added -fn option to template command to allow for templated output file names.

Added -f option to shuffle command to allow shuffling of fields.

Added several new functions for use by expression language.

The random number generator used by the expression language can be seeded with the -seed option.

Can now use != as well as <> for not-equal operator in expression language.

Output of help now goes to standard output, not standard error.

Removed the -rin option as the idea behind it was basically wrong.


&lt;hr&gt;



<b>
Changes from Version 1.3 to 1.4<br>
</b>

Added timestamp command.

Added call command to call function in external DLL.

Added money command to do currency formatting.

Added -rf option to exclude command to allow specifying of fields from end of record.

Added -d option to eval command to discard input data

Added -h option to odbc\_get command to get column names as CSV header record

Fixed -s option bug for find and remove commands

The validate command now supports multiple error reports for the same record.

Added -rin flag  to make use of expressions mixing numbers and strings a bit easier.

Added -rh flag to sort command to retain header record on output
Various updates to the manual.


&lt;hr&gt;



<b>
Changes from Version 1.2 to 1.3<br>
</b>

Added block command to mark blocks of records.

Added stat command to produce CSV file record/field counts.

Improved error reporting for commands using the expression language.

Many bug fixes.


&lt;hr&gt;


<br><b>
Changes from Version 1.1 to 1.2<br>
</b>

Command name abbreviations are now supported.<br>
<br>
Added check command to do CSV syntactic validation.<br>
<br>
The odbc_get command now has a -dir option to simplify using the ODBC text driver.<br>
<br>
The map command now supports dynamically mapping to other fields in the input record.<br>
<br>
Added match() function to match regular expressions to eval command.<br>
<br>
Added -r flag to eval command to allow field replacement as well as appending.<br>
<br>
The find and remove commands now support field testing via eval-style expressions.<br>
<br>
The exclude command now supports excluding fields based on an eval-style expression.<br>
<br>
The -si flag in the find and remove commands now works correctly.<br>
<br>
<br>
<hr><br>
<br>
<br>
<br><br>
<b>
Changes from Version 1.0 to 1.1<br>
</b>

The find and remove commands now support non-regular expression strings.<br>
<br>
<br>
Added diff command for comparing CSV files.<br>
<br>
Added file_merge command for merging sorted files.<br>
<br>
The split_char command now supports splitting on character type transitions.<br>
<br>
Added rmnew command to remove newlines inside CSV fields.<br>
<br>
Help now implemented using HelpNDoc - looks nicer & spelling mistakes all (?) fixed.<br>
<br>
<br>
<hr><br>
<br>
<br>
<br>
<b>
Changes from Version 0.97 to 1.0<br>
</b>


Added the from_xml command to convert XML data to CSV. This is a much improved facility over the original read_xml command, which was removed a while back.<br>
<br>
Added ability to search for ranges and lengths with the find and remove commands.<br>
<br>
Commands like order now support field indexes specified as ranges.<br>
<br>
Fixed problem with trailing empty fields being ignored.<br>
<br>
Added -nc flag to order command to allow skipping of non-existent input fields<br>
<br>
Added new functions to eval command.<br>
<br>
Added -d flag to allow decremented numbers for sequence command.<br>
<br>
Added -fc flag to find and remove commands to allow search on CSV field counts.<br>
<br>
The put command can now output field counts.