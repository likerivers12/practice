{ printf("%d ", CLASS)

	i=1
	while ( i <= NF ) {
		printf("%d:%f ", i, $i)
		i=i+1
	}
	printf("\n")
}
#{ printf("%d 1:%f 2:%f 3:%d 4:%d 5:%d 6:%f 7:%f\n", CLASS, $1, $2, $3, $4, $5, $6, $7) } 
#
#{ printf("%d 1:%f 2:%f 3:%d 4:%f 5:%f\n", CLASS, $1, $2, $3, $4, $5) } 
#
#
#
#{ print $1 $2 }
#{ print NF }
#{ print ARGV[2] }
#{ print CLASS }
