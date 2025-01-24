# Load lpSolve
library(lpSolve)

# Define the cost matrix
# costs <- c(19, 30, 50, 10,
#            70, 30, 40, 60,
#            40, 8, 70, 20)
# supply <- c(7, 9, 18)
# demand <- c(5, 8, 7, 14)

# costs <- c(
# 	16, 16, 13, 22, 17,
# 	14, 14, 13, 19, 15,
# 	19, 19, 20, 23, 10,
# 	0, 0, 0, 0, 0
# )
# supply <- c(50, 60, 50, 50)
# demand <- c(30, 20, 70, 30, 60)

# demand <- c(5,	8,	7,	14)
# supply <- c(7, 9, 18)
# costs = c(
# 	 19,	30,	50,	10 ,
# 	 70,	30,	40,	60 ,
# 	 40,	8,	70,	20 
# )

demand <- c(5,	8,	7,	14)
supply <- c(7, 9, 18)
costs = c(
	51, 40, 20, 60,
	0, 40, 30, 10,
	30,62, 0, 50
)

# costs <- c(
# 	8, 4, 1, 2, 1, 0, 
# 	7, 1, 7, 3, 8, 0,
# 	3, 7, 8, 7, 5, 0,
# 	1, 3, 6, 7, 2, 0
# )
# demand <- c(50, 10, 65, 25, 10, 35)
# supply <- c(40, 60, 70, 25)

# The number of suppliers and demand points
n_supply <- length(supply)
n_demand <- length(demand)

# Create the constraint matrix
constraint_matrix <- matrix(0, nrow = n_supply + n_demand, ncol = n_supply * n_demand)

# Supply constraints
for (i in 1:n_supply) {
  constraint_matrix[i, ((i - 1) * n_demand + 1):(i * n_demand)] <- 1
}

# Demand constraints
for (j in 1:n_demand) {
  indices <- seq(from = j, to = n_supply * n_demand, by = n_demand)
  constraint_matrix[n_supply + j, indices] <- 1
}

# Combine supply and demand into a single vector
rhs <- c(supply, demand)

# Constraint types: all constraints are equality
constraint_types <- rep("=", n_supply + n_demand)

# Solve the linear program
lp_solution <- lp("min", costs, constraint_matrix, constraint_types, rhs, all.int = TRUE)

# Check the solution status
if (lp_solution$status == 0) {
  optimal_values <- matrix(lp_solution$solution, nrow = n_supply, byrow = TRUE)
  print("Optimal transportation quantities:")
  print(optimal_values)
  
  # Calculate the optimal cost
  optimal_cost <- sum(optimal_values * matrix(costs, nrow = n_supply, byrow = TRUE))
  cat("Optimal cost:", optimal_cost, "\n")
} else {
  print("No optimal solution found")
}
