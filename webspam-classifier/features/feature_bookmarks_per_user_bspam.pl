my %table;

open(TAS_SPAM,"tas_spam") or die "cant open TAS_SPAM: $!";
open(TSPAM,">bookmarks_per_user_spam") or die "cant open TSPAM: $!";

while(<TAS_SPAM>){
	chomp;
	my ($user_id , $tag , $content_id, $content_val ,$date) = split /\s+/;
	push @{$table{$user_id}},$content_id;
}

foreach $user_id (sort {$a <=> $b } keys %table){
	
		if ( $user_id % 32 == 0) {
		
		@values = @{$table{$user_id}};
		$count = @values;		
		
			print " $user_id : $count\n ";
		
			print TSPAM " $user_id : $count \n"; 
		}
		

}

close(TAS_SPAM);
close(TSPAM);


