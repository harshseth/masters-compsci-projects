my %table;
my %table1;
$count = 0;
$usercount = 0;
$tagcount=0;
$loopcount = 0;
$valueslen = 0;

open(TAS,"tas") or die "cant open TAS: $!";
open(TS,">tas_special_chars") or die "cant open TS: $!";

while(<TAS>){
	chomp;
	my ($user_id , $tag, $content_id, $content_value, $date) = split /\t+/;
	push @{$table{$user_id}} , $tag; 
	
}

foreach $user_id (sort { $a <=> $b } keys %table){
	
		my $loopcount = 0;
		if(	$user_id % 3 == 0 && $user_id !=0) { 
		
		
		@values = @{$table{$user_id}};
		$valueslen = @values;
		#print "$valueslen:";
		$count = 0;
		$lencount = 0;
		$loopcount = 0;
		print "$user_id ";	
		foreach $_ (@values) {
			
			$loopcount++;
			if($_ =~ m/(\$|\=|\-|\_|\+|\\|\/|\}|\{|\<|\>|\!|\#|\@|\%|\^|\"|\:|\;|\~|\(|\)|\?|\.|\,|\')|[1-9]/ ) {
			$count++;
			$lencount++;
			#print "$lencount:$loopcount:";
			print " ($_ ) ";
			print TS "$user_id\n";
				
			}
				if ($loopcount == $valueslen ){
				if( $lencount == 0 ) {	print TS "$user_id\n";}
				if( $lencount == 1 ) {	print TS "$user_id\n";}
				}	

	
		}
			print " ($count) ";

			print "\n\n\n\n";
	 

}
		

}

print "count: $count\n";

close(TS);
open(TS,"tas_special_chars") or die "cant open TS: $!";

open(TS1,">train_TS") or die "cant open TS1: $!";

while(<TS>){
	chomp;
	my ($user_id1) = split /\s+/;
	push @{$table1{$user_id1}} , 0; 
	

}

foreach $user_id1 (sort {$a <=> $b}  keys %table1){

	$tagcount = @{$table1{$user_id1}};
	if ($tagcount == 1) {$tagcount = 0;}
	$usercount++;
	print TS1 "$user_id1 ";
	print TS1 "-1 ";
	print TS1 "1:$tagcount";
	print TS1 "\n";
	print "$user_id1: $tagcount";
	print "\n";

}

close(TS1);
close(TS);
close(TAS);
