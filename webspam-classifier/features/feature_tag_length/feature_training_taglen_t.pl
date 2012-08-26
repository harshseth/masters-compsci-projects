my %table;
my %table1;
$count = 0;
$usercount = 0;
$tagcount=0;
$loopcount = 0;
$valueslen = 0;
open(TAS,"tas") or die "cant open TAS: $!";
open(TS2,">tas_tag_len") or die "cant open TS2: $!";

while(<TAS>){
	chomp;
	my ($user_id , $tag, $content_id, $content_value, $date) = split /\t+/;
	push @{$table{$user_id}} , $tag; 
	#$table{$content_id} = $user_id;

}

foreach $user_id (sort { $a <=> $b} keys %table){
	
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
			if(length($_) <= 2) {
			$count++;
			$lencount++;
		#	print "$lencount:$loopcount:";
			print " ($_ )";
			print TS2 "$user_id\n";
				
			}
				if ($loopcount == $valueslen ){
				if( $lencount == 0 ) {	print TS2 "$user_id\n";}
				if( $lencount == 1 ) {	print TS2 "$user_id\n";}
				}	

	#	print "$user_id : $count "
		}	
			print " ($count) ";
			print "\n\n\n\n";
	#print TSPAM "$user_id : $count "; 
	 
	#		print TSPAM "\n\n\n\n"; 
	#		print "\n\n\n\n";	
		
}
		

}

print "count: $count\n";

close(TS2);
open(TS2,"tas_tag_len") or die "cant open TS2: $!";

open(TS1,">train_tag_len_T") or die "cant open TS1: $!";

while(<TS2>){
	chomp;
	my ($user_id1) = split /\s+/;
	push @{$table1{$user_id1}} , 0; 
	#$table{$content_id} = $user_id;

}

foreach $user_id1 (sort { $a <=> $b } keys %table1){

	$tagcount = @{$table1{$user_id1}};
	if ($tagcount == 1) {$tagcount = 0;}
	$usercount++;
	#print TSPAM1 "+1 ";
	print TS1 "$user_id1 ";
	print TS1 "3:$tagcount";
	print TS1 "\n";
	print "$user_id1: $tagcount";
	print "\n";

}
#print "$usercount\n";
close(TS1);
close(TS2);
close(TAS);
