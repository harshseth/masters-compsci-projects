my %table;

open(TAS_SPAM,"bookmark_output") or die "cant open TAS_SPAM: $!";
open(TSPAM,">users_per_bookmark") or die "cant open TSPAM: $!";

while(<TAS_SPAM>){
	chomp;
	my ($user_id , $content_id , $url_hash, $url , $description , $extended , $date) = split /\s+/;
	push @{$table{$content_id}},$user_id;
	
}

foreach $content_id (sort { $a <=> $b } keys %table){
	
		
		
		@values = @{$table{$content_id}};
		$count = @values;		
		
			print "$content_id : $count \n\n\n";
		
			print TSPAM "$content_id : $length\n\n\n"; 
	
		

}

close(TAS_SPAM);
close(TSPAM);


