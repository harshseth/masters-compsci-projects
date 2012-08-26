my %table;

open(TAS_SPAM,"bookmark_output_spam") or die "cant open TAS_SPAM: $!";
open(TSPAM,">users_per_bookmark_spam") or die "cant open TSPAM: $!";

while(<TAS_SPAM>){
	chomp;
	my ($user_id , $content_id , $url_hash, $url , $description , $extended , $date) = split /\s+/;
	push @{$table{$content_id}},$user_id;
	#push @{$table{user_id}},$content_id;
}

foreach $content_id (keys %table){
	
		
		
		@values = @{$table{$content_id}};
		$length = @values;		
		
			print "$content_id : $length \n\n\n";
		
			print TSPAM "$content_id : $length\n\n\n"; 
	
		

}


close(TAS_SPAM);
close(TSPAM);


