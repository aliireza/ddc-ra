#This script will analyze the data from DDC RA

#set the working directory to the folder that data exist counts <- table(mtcars$gear)

setwd("/Users/eamiroo/Documents/constraint-programming/Projects/DDC-RA/results-20VM/")

#make the folder for graphs
plotDir= paste(getwd(),"/Plot/", sep="")
if (!file.exists(plotDir)){
  dir.create(plotDir)
}

# rading a data fo cluster
cl_cp=read.csv(file = "cluster-data.csv.cp", header = TRUE, sep=",")
cl_ff=read.csv(file = "cluster-data.csv.ff", header = TRUE, sep=",")
#redaing data for Logical servers
ls_cp=read.csv(file = "logicalServer-data.csv.cp", header = TRUE, sep=",")
ls_ff=read.csv(file = "logicalServer-data.csv.ff", header = TRUE, sep=",")

#normlized cp vilation
cp_nrm_LocalSLAViolation= (sum(ls_cp$LocalSLAViolation) / nrow(ls_cp) ) *100
cp_nrm_RemoteSLAViolatio= (sum(ls_cp$RemoteSLAViolation) / nrow(ls_cp) ) * 100
cp_nrm_totalViolation = (sum(ls_cp$LocalSLAViolation | ls_cp$RemoteSLAViolation) / nrow(ls_cp) ) * 100 # either of vilation should be counted

#normalized first fit violation
ff_nrm_LocalSLAViolation= (sum(ls_ff$LocalSLAViolation) / nrow(ls_ff) ) *100
ff_nrm_RemoteSLAViolatio= (sum(ls_ff$RemoteSLAViolation) / nrow(ls_ff) ) * 100
ff_nrm_totalViolation = (sum(ls_ff$LocalSLAViolation | ls_ff$RemoteSLAViolation) / nrow(ls_ff) ) * 100 # either of vilation should be counted

# we should decide how to visulize the price!!!


# make a matrix for violation 
#make teh array from result
tmp=c(ff_nrm_LocalSLAViolation,cp_nrm_LocalSLAViolation,ff_nrm_RemoteSLAViolatio, cp_nrm_RemoteSLAViolatio, ff_nrm_totalViolation, cp_nrm_totalViolation)
#make matrix 
resviolations=matrix(tmp, nrow = 2, ncol = 3)
colnames(resviolations) <- c("LocalSLA", "RemoteSLA", "Total")
rownames(resviolations) <- c("FF", "CP")

##Ploting 
# Open a pdf file
pdf(paste(plotDir,"violation.pdf", sep=""))
# 2. Create a plot
barplot(resviolations, col = c("red", "green"), beside = TRUE, xlab="Violation",
        ylab = "Percentage", legend = rownames(resviolations))
# Close the pdf file
dev.off() 

#Price Plotting

# get all allocation without violation
Mix = cbind("id"=ls_ff$id, "FF_Price"=ls_ff$price, "CP_Price"=ls_cp$price, "FF_Viol"=(ls_ff$LocalSLAViolation + ls_ff$RemoteSLAViolation), "CP_Viol"=(ls_cp$LocalSLAViolation + ls_cp$RemoteSLAViolation) , "Total"=(ls_cp$LocalSLAViolation + ls_cp$RemoteSLAViolation + ls_ff$LocalSLAViolation + ls_ff$RemoteSLAViolation) )
Mix=cbind(Mix, "FFMORE"=(Mix[,"FF_Price"] > Mix[,"CP_Price"]))

meetSLA=Mix[Mix[,"Total"] %in% 0,]
meetSLA=cbind(meetSLA, "FFMORE"=(meetSLA[,"FF_Price"] > meetSLA[,"CP_Price"])) # FFMORE are the cases the FF price is more than CP price for same LS - Note in this case LS in both senarios meets LSAs
meetSLA=cbind(meetSLA, "PExpensive"=((meetSLA[,"FF_Price"] - meetSLA[,"CP_Price"])/meetSLA[,"CP_Price"] )*100)

tmp=c( (sum(Mix[,"FFMORE"])/nrow(Mix))*100 , (sum(meetSLA[,"FFMORE"])/nrow(meetSLA))*100  )
#make matrix 
mat=matrix(tmp, nrow = 2, ncol = 1)
colnames(mat) <- c("of")
rownames(mat) <- c("All cases", "SLA aproved")

##Ploting 
# Open a pdf file
pdf(paste(plotDir,"Price.pdf", sep=""))
# 2. Create a plot
barplot(tmp,
        main = paste("Cases that FF result in higher price compared to CP"),
        sub =  paste("Minimum overparice", round(min(meetSLA[,"PExpensive"])), "% and Maximum overparice is",round(max(meetSLA[,"PExpensive"])), "%"),
        xlab = "scenarios",
        ylab = "Percentage",
        names.arg = c("All allocataed LS", "LS with SLA approved"),
        col = c("green", "red")
)
# Close the pdf file
dev.off() 


#cluster data

# get the clusters that are ON
cl_cp_filter=cl_cp[cl_cp$cluster_utilization != 0,]

cl_ff_filter=cl_ff[cl_ff$cluster_utilization != 0,]

if (nrow(cl_ff_filter) > nrow(cl_cp_filter)){
  cl_ff=cl_ff_filter
  cl_cp=cl_cp[1:nrow(cl_ff),]
}else{
  cl_cp=cl_cp_filter
  cl_ff=cl_ff[1:nrow(cl_cp),]
}

tmp=cbind(cl_cp$cluster_utilization, cl_ff$cluster_utilization, cl_cp$cpu_utilization, cl_ff$cpu_utilization, cl_cp$memory_utilization, cl_ff$memory_utilization)
#mat=matrix(tmp, nrow = 12, ncol = 2)
##Ploting 
# Open a pdf file
pdf(paste(plotDir,"utilization.pdf", sep=""))
# 2. Create a plot
boxplot(tmp,
        main = paste("Utilization of DC and each pool"),
        at =c(1,2, 4,5, 7,8),
        #sub =  paste("Minimum overparice", round(min(meetSLA[,"PExpensive"])), "% and Maximum overparice is",round(max(meetSLA[,"PExpensive"])), "%"),
        xlab = "Parameters",
        ylab = "Utilization (%)",
        names = c("DC utilization", "DC utilization", "CPU utilization", "CPU utilization", "Memory utilization", "Memory utilization"),
        col = c("green", "red")
)

legend("bottomleft", inset=.02, title="Resource allocator",
       c("CP","FF"), fill=c("green","red"), horiz=TRUE, cex=0.8)
# Close the pdf file
dev.off() 

#utilization per memory tier
#CP
local_mem_util_cp= 100 * cl_cp$used_local_memory / cl_cp$total_local_memory
remote_local_mem_util_cp = 100 * cl_cp$used_local_remote_memory / cl_cp$total_local_remote_memory
remote_remote_mem_util_cp = 100 * cl_cp$used_remote_remote_memory / cl_cp$total_remote_remote_memory
#FF
local_mem_util_ff= 100 * cl_ff$used_local_memory / cl_ff$total_local_memory
remote_local_mem_util_ff = 100 * cl_ff$used_local_remote_memory / cl_ff$total_local_remote_memory
remote_remote_mem_util_ff = 100 * cl_ff$used_remote_remote_memory / cl_ff$total_remote_remote_memory

tmp= cbind(local_mem_util_cp, local_mem_util_ff, remote_local_mem_util_cp, remote_local_mem_util_ff, remote_remote_mem_util_cp, remote_remote_mem_util_ff )

#Plot

##Ploting 
# Open a pdf file
pdf(paste(plotDir,"Mem_Pools_utilization.pdf", sep=""))
# 2. Create a plot
boxplot(tmp,
        main = paste("Utilization for each Memory Tier"),
        at =c(1,2, 4,5, 7,8),
        #sub =  paste("Minimum overparice", round(min(meetSLA[,"PExpensive"])), "% and Maximum overparice is",round(max(meetSLA[,"PExpensive"])), "%"),
        xlab = "Parameters",
        ylab = "Utilization (%)",
        names = c("Local Memory Util", "", "Local Remote Memory", "", "Remote Remote Memory", ""),
        col = c("green", "red")
)

legend("bottomleft", inset=.02, title="Resource allocator",
       c("CP","FF"), fill=c("green","red"), horiz=TRUE, cex=0.8)
# Close the pdf file
dev.off() 