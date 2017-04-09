/*
 *	题目：输入一个整形数组，数组里有正数也有负数。数组中连续的一个或多个整数组成一个子数组，每个子数组都有一个和。
 *	求所有子数组的和的最大值。要求时间复杂度为O(n)。
 *
 *	例如输入的数组为1, -2, 3, 10, -4, 7, 2, -5，和最大的子数组为3, 10, -4, 7, 2，因此输出为该子数组的和18。
 *
 * 	假如数组中全为正数，那么最大和必然为全部数相加；如果数组中有负数，并且如果加上某个负数，子数组的和小于0，则最大和子数组必然不包含这个负数。
 *
 *	author: louluo
 */
