my %table;
$usercount = 0;
$usercount1 = 0;
$usercountreal = 0;
open(TASSPAM,"bookmark_output_spam") or die "cant open TASSPAM: $!";

while(<TASSPAM>){
	$usercountreal++;
	chomp;
	my ($user_id , $content_id, $hash, $url, $desc, $date) = split /\s+/;
	$table{$user_id} = FALSE;
  
}


foreach $user_id (sort { $a <=> $b } keys %table){
	
		$usercount++;
		if(	$user_id % 32 == 0) { 
		$usercount1++;
		$table{$user_id} = TRUE;
 
		}
		
		print "$user_id   "; 		
		print "$table{$user_id}";
		print "\n";
		

}



print "$usercountreal $usercount  $usercount1 \n";

close(TASSPAM);

